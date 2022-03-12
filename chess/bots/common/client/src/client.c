static const uint8_t client_GET_REQUEST[] = "GET /chess HTTP/1.1\r\nSec-WebSocket-Key: AQIDBAUGBwgJCgsMDQ4PEC==\r\n\r\n";

static inline void client_create(struct client *self, client_makeMove makeMove) {
    self->received = 0;
    self->makeMove = makeMove;
}

// Return is same as recv().
static int32_t client_receive(struct client *self) {
    int32_t status = (int32_t)hc_recvfrom(self->socketFd, &self->receiveBuffer[self->received], client_RECEIVE_BUFFER_SIZE - self->received, 0, NULL, NULL);
    if (status > 0) self->received += status;
    return status;
}

static int32_t client_receiveWebsocket(struct client *self, uint8_t **payload, int32_t *length) {
    while (self->received < 2) if (client_receive(self) <= 0) return -1;
    // Assumes no mask, no fragmentation and single byte payload length.
    *payload = &self->receiveBuffer[2];
    *length = self->receiveBuffer[1] & 0x7F;
    while (self->received < 2 + *length) if (client_receive(self) <= 0) return -2;
    return 0;
}

static inline void client_ack(struct client *self, int32_t length) {
    self->received -= length;
    hc_MEMMOVE(&self->receiveBuffer[0], &self->receiveBuffer[length], (uint64_t)self->received);
}

static inline void client_ackWebsocket(struct client *self) {
    client_ack(self, 2 + (self->receiveBuffer[1] & 0x7F));
}

static int32_t client_sendWebsocket(struct client *self, int32_t length) {
    if (length >= 126) return -1;
    uint8_t frame[6] = { 0x82, (uint8_t)(0x80 | length), 0, 0, 0, 0 };

    struct iovec iov[2] = {
        {
            .iov_base = &frame[0],
            .iov_len = 6
        }, {
            .iov_base = &self->sendBuffer[0],
            .iov_len = length
        }
    };
    struct msghdr msg = {
        .msg_iov = &iov[0],
        .msg_iovlen = 2,
    };
    if (hc_sendmsg(self->socketFd, &msg, MSG_NOSIGNAL) != 6 + length) return -2;
    return 0;
}

static int32_t client_onChessUpdate(struct client *self, uint8_t *payload, hc_UNUSED int32_t length) {
    uint8_t winner = payload[2];
    if (winner != protocol_NO_WIN) return 0; // Game already over.

    bool hostsTurn = payload[1];
    if (hostsTurn == self->state.wasHostsTurn) return 0; // No turn change.
    self->state.wasHostsTurn = hostsTurn;

    if (self->state.isHost != hostsTurn) return 0; // Not our turn.

    int32_t moveFrom;
    int32_t moveTo;
    int32_t status = self->makeMove(self->state.isHost, &payload[21], payload[3], payload[4], &moveFrom, &moveTo);
    if (status < 0) {
        debug_printNum("Bot failed to make a move: ", status, "\n");
        return -1;
    }

    debug_printNum("Bot tries to move from ", moveFrom, " ");
    debug_printNum("to ", moveTo, ". ");
    debug_printNum("(", moveFrom % 8, ", ");
    debug_printNum("", moveFrom / 8, ")->");
    debug_printNum("(", moveTo % 8, ", ");
    debug_printNum("", moveTo / 8, ")\n");
    self->sendBuffer[0] = protocol_MOVE;
    self->sendBuffer[1] = (uint8_t)moveFrom;
    self->sendBuffer[2] = (uint8_t)moveTo;

    if (client_sendWebsocket(self, 3) < 0) return -2;
    return 0;
}

static int32_t client_run(struct client *self, uint8_t *address, uint16_t port, int32_t roomId) {
    self->state.isHost = roomId < 0;

    self->socketFd = hc_socket(AF_INET, SOCK_STREAM, 0);
    if (self->socketFd < 0) return -1;

    int32_t enable = 1;
    int32_t status = hc_setsockopt(self->socketFd, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(enable));
    if (status < 0) {
        status = -2;
        goto cleanup_socket;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = hc_BSWAP16(port);
    hc_MEMCPY(&serverAddr.sin_addr.s_addr, address, 4);

    status = hc_connect(self->socketFd, &serverAddr, sizeof(serverAddr));
    if (status < 0) {
        status = -3;
        goto cleanup_socket;
    }

    if (hc_sendto(self->socketFd, &client_GET_REQUEST[0], sizeof(client_GET_REQUEST) - 1, MSG_NOSIGNAL, NULL, 0) != sizeof(client_GET_REQUEST) - 1) {
        status = -4;
        goto cleanup_socket;
    }

    // Wait for response.
    int32_t responseLength;
    for (;;) {
        status = client_receive(self);
        if (status <= 0) {
            status = -5;
            goto cleanup_socket;
        }

        for (int32_t i = 0; i + 3 < self->received; ++i) {
            if (
                self->receiveBuffer[i] == '\r' &&
                self->receiveBuffer[i + 1] == '\n' &&
                self->receiveBuffer[i + 2] == '\r' &&
                self->receiveBuffer[i + 3] == '\n'
            ) {
                responseLength = i + 4; // Assume no body.
                goto gotResponse;
            }
        }
    }
    gotResponse:
    debug_printStr("Http response:\n", (char *)&self->receiveBuffer[0], "", responseLength);
    client_ack(self, responseLength);

    // Get protocol version.
    uint8_t *payload;
    int32_t length;
    if (
        client_receiveWebsocket(self, &payload, &length) < 0 ||
        length != 4
    ) {
        status = -6;
        goto cleanup_socket;
    }
    int32_t protocolVersion;
    hc_MEMCPY(&protocolVersion, &payload[0], 4);
    client_ackWebsocket(self);

    if (protocolVersion != protocol_VERSION) {
        status = -7;
        goto cleanup_socket;
    }

    // Run main loop.
    for (;;) {
        if (client_receiveWebsocket(self, &payload, &length) < 0) {
            status = -8;
            goto cleanup_socket;
        }
        switch (payload[0]) {
            case protocol_HOME: {
                debug_printStr("Home view!\n", "", "", 0);
                self->state.wasHostsTurn = false;
                if (self->state.isHost) {
                    self->sendBuffer[0] = protocol_CREATE;
                    if (client_sendWebsocket(self, 1) < 0) {
                        status = -9;
                        goto cleanup_socket;
                    }
                } else {
                    self->sendBuffer[0] = protocol_JOIN;
                    hc_MEMCPY(&self->sendBuffer[1], &roomId, 4);
                    if (client_sendWebsocket(self, 5) < 0) {
                        status = -10;
                        goto cleanup_socket;
                    }
                }
                break;
            }
            case protocol_ROOM: {
                int32_t id;
                hc_MEMCPY(&id, &payload[1], 4);
                debug_printNum("Created room with id: ", id, "\n");
                break;
            }
            case protocol_CHESS: {
                status = client_onChessUpdate(self, payload, length);
                if (status < 0) {
                    debug_printNum("Chess update failed: ", status, "\n");
                    status = -11;
                    goto cleanup_socket;
                }
                break;
            }
            default: hc_UNREACHABLE;
        }
        client_ackWebsocket(self);
    }
    status = 0;

    cleanup_socket:
    hc_close(self->socketFd);
    return status;
}
