#define server_WEBSOCKET_ACCEPT_START \
    "HTTP/1.1 101 Switching Protocols\r\n" \
    "Upgrade: websocket\r\n" \
    "Connection: Upgrade\r\n" \
    "Sec-WebSocket-Accept: "
#define server_WEBSOCKET_ACCEPT_START_LEN (34 + 20 + 21 + 22)

static int32_t server_init(
    struct server *self,
    uint16_t port,
    struct fileResponse *fileResponses,
    int32_t fileResponsesLength,
    struct serverCallbacks *callbacks
) {
    self->fileResponses = fileResponses;
    self->fileResponsesLength = fileResponsesLength;
    self->callbacks = *callbacks;

    for (int32_t i = 0; i < server_MAX_CLIENTS; ++i) {
        serverClient_init(&self->clients[i], i);
    }

    self->listenSocketFd = nolibc_socket(AF_INET, SOCK_STREAM, 0);
    if (self->listenSocketFd < 0) return -1;

    int32_t status;
    int32_t enable = 1;
    if (nolibc_setsockopt(self->listenSocketFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
        status = -2;
        goto cleanup_listenSocketFd;
    }
    if (nolibc_setsockopt(self->listenSocketFd, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(enable)) < 0) {
        status = -3;
        goto cleanup_listenSocketFd;
    }

    struct sockaddr_in listenAddr;
    listenAddr.sin_family = AF_INET;
    listenAddr.sin_addr.s_addr = INADDR_ANY;
    listenAddr.sin_port = (uint16_t)((port >> 8) | (port << 8)); // Byteswap

    if (nolibc_bind(self->listenSocketFd, &listenAddr, sizeof(listenAddr)) < 0) {
        status = -4;
        goto cleanup_listenSocketFd;
    }

    if (nolibc_listen(self->listenSocketFd, 128) < 0) {
        status = -5;
        goto cleanup_listenSocketFd;
    }

    self->epollFd = nolibc_epoll_create1(0);
    if (self->epollFd < 0) {
        status = -6;
        goto cleanup_listenSocketFd;
    }

    struct epoll_event listenSocketEvent = {
        .events = EPOLLIN,
        .data.ptr = &self->listenSocketFd
    };
    if (nolibc_epoll_ctl(self->epollFd, EPOLL_CTL_ADD, self->listenSocketFd, &listenSocketEvent) < 0) {
        status = -7;
        goto cleanup_epollFd;
    }

    self->sha1SocketFd = nolibc_socket(AF_ALG, SOCK_SEQPACKET, 0);
    if (self->sha1SocketFd < 0) {
        status = -8;
        goto cleanup_epollFd;
    }
    struct sockaddr_alg sha1Addr = {
        .salg_family = AF_ALG,
        .salg_type = "hash",
        .salg_name = "sha1"
    };
    if (nolibc_bind(self->sha1SocketFd, &sha1Addr, sizeof(sha1Addr)) < 0) {
        status = -9;
        goto cleanup_sha1SocketFd;
    }
    self->sha1InstanceFd = nolibc_accept4(self->sha1SocketFd, NULL, NULL, 0);
    if (self->sha1InstanceFd < 0) {
        status = -10;
        goto cleanup_sha1SocketFd;
    }

    return 0;

    cleanup_sha1SocketFd:
    nolibc_close(self->sha1SocketFd);
    cleanup_epollFd:
    nolibc_close(self->epollFd);
    cleanup_listenSocketFd:
    nolibc_close(self->listenSocketFd);
    return status;
}

static inline void server_deinit(struct server *self) {
    for (int32_t i = 0; i < server_MAX_CLIENTS; ++i) {
        serverClient_deinit(&self->clients[i]);
    }
    nolibc_close(self->sha1InstanceFd);
    nolibc_close(self->sha1SocketFd);
    nolibc_close(self->epollFd);
    nolibc_close(self->listenSocketFd);
}

static int32_t server_acceptSocket(struct server *self) {
    int32_t newSocketFd = nolibc_accept4(self->listenSocketFd, NULL, NULL, SOCK_NONBLOCK);
    if (newSocketFd < 0) goto cleanup_none;

    int32_t enable = 1;
    if (nolibc_setsockopt(newSocketFd, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(enable)) < 0) goto cleanup_newSocketFd;

    // Find empty client spot
    for (int32_t i = 0; i < server_MAX_CLIENTS; ++i) {
        if (self->clients[i].fd < 0) {
            struct epoll_event newSocketEvent;
            newSocketEvent.events = EPOLLIN;
            newSocketEvent.data.ptr = &self->clients[i];

            if (nolibc_epoll_ctl(self->epollFd, EPOLL_CTL_ADD, newSocketFd, &newSocketEvent) < 0) goto cleanup_newSocketFd;

            serverClient_open(&self->clients[i], newSocketFd);
            return 0;
        }
    }
    // Max clients reached
    nolibc_close(newSocketFd);
    return 1;

    cleanup_newSocketFd:
    nolibc_close(newSocketFd);
    cleanup_none:
    return -1;
}

static int32_t server_findLineEnd(uint8_t *buffer, int32_t index, int32_t end) {
    for (; index + 1 < end; ++index) {
        if (buffer[index] == '\r' && buffer[index + 1] == '\n') return index;
    }
    return -1;
}

static int32_t server_sendWebsocketMessage(struct server *self, struct serverClient *client, uint8_t *message, int32_t messageLength, bool isText) {
    if (isText) self->scratchSpace[0] = 0x81;
    else self->scratchSpace[0] = 0x82;

    int32_t headerLength;
    if (messageLength < 126) {
        self->scratchSpace[1] = (uint8_t)messageLength;
        headerLength = 2;
    } else {
        self->scratchSpace[1] = 126;
        self->scratchSpace[2] = (uint8_t)(messageLength >> 8);
        self->scratchSpace[3] = (uint8_t)(messageLength & 0xFF);
        headerLength = 4;
    }

    struct iovec iov[2] = {
        {
            .iov_base = &self->scratchSpace[0],
            .iov_len = headerLength
        }, {
            .iov_base = message,
            .iov_len = messageLength
        }
    };
    struct msghdr msg = {
        .msg_iov = &iov[0],
        .msg_iovlen = 2,
    };
    if (nolibc_sendmsg(client->fd, &msg, MSG_NOSIGNAL) != headerLength + messageLength) return -1;
    return 0;
}

// Returns 1 if the connection should be kept.
static int32_t server_handleWebsocket(struct server *self, struct serverClient *client) {
    if (client->receiveLength < 2) return 1; // No space for even the payload length.
    if (!(client->receiveBuffer[0] & 0x80)) return -1; // We don't support fragmentation.

    int32_t opcode = client->receiveBuffer[0] & 0x0F;
    if (!(client->receiveBuffer[1] & 0x80)) return -2; // Require masking.

    uint64_t payloadLength = client->receiveBuffer[1] & 0x7F;
    int32_t maskingKeyIndex;
    if (payloadLength == 126) {
        if (client->receiveLength < 4) return 1;
        payloadLength = (
            ((uint64_t)client->receiveBuffer[2] << 8) |
            ((uint64_t)client->receiveBuffer[3])
        );
        maskingKeyIndex = 4;
    } else if (payloadLength == 127) {
        if (client->receiveLength < 10) return 1;
        payloadLength = (
            ((uint64_t)client->receiveBuffer[2] << 54) |
            ((uint64_t)client->receiveBuffer[3] << 48) |
            ((uint64_t)client->receiveBuffer[4] << 40) |
            ((uint64_t)client->receiveBuffer[5] << 32) |
            ((uint64_t)client->receiveBuffer[6] << 24) |
            ((uint64_t)client->receiveBuffer[7] << 16) |
            ((uint64_t)client->receiveBuffer[8] << 8) |
            ((uint64_t)client->receiveBuffer[9])
        );
        maskingKeyIndex = 10;
    } else maskingKeyIndex = 2;
    int32_t payloadStart = maskingKeyIndex + 4;
    if ((uint64_t)(client->receiveLength - payloadStart) < payloadLength) return 1; // Have we recieved everything? Also ensures we can cast payloadLength to int32_t.

    for (int32_t i = 0; i < (int32_t)payloadLength; ++i) {
        client->receiveBuffer[payloadStart + i] ^= client->receiveBuffer[maskingKeyIndex + (i % 4)];
    }

    switch (opcode) {
        case 0x2: { // Binary
            if (self->callbacks.onMessage(self->callbacks.data, client, &client->receiveBuffer[payloadStart], (int32_t)payloadLength) != 0) return -3;
            break;
        }
        case 0x8: { // Close
            // TODO should probably send a close frame.
            return 0;
        }
        case 0x9: { // Ping
            // TODO reply
            break;
        }
        case 0xA: { // Pong
            // TODO callback?
            break;
        }
        default: { // TODO: Text, continuation frames
            return -4;
        }
    }
    client->receiveLength = 0;
    return 1;
}

// Returns 1 if the connection should be kept.
static int32_t server_handleHttpRequest(struct server *self, struct serverClient *client) {
    int32_t firstLineEnd = server_findLineEnd(client->receiveBuffer, 0, client->receiveLength);
    if (firstLineEnd < 0) return 1;
#define server_GET_SLASH_LEN 5
    bool isGet = (firstLineEnd >= server_GET_SLASH_LEN && nolibc_MEMCMP(client->receiveBuffer, "GET /", server_GET_SLASH_LEN) == 0);
    int32_t websocketKeyStart = 0;
    int32_t websocketKeyLength;

    int32_t currentLine = firstLineEnd + 2;
    for (;;) {
        int32_t lineEnd = server_findLineEnd(client->receiveBuffer, currentLine, client->receiveLength);
        if (lineEnd < 0) return 1;

        int32_t lineLength = lineEnd - currentLine;
        if (lineLength == 0) break;

        if (lineLength > 18 && nolibc_MEMCMP(&client->receiveBuffer[currentLine], "Sec-WebSocket-Key:", 18) == 0) {
            int32_t i = currentLine + 18;
            for (; i < currentLine + lineLength; ++i) {
                if (websocketKeyStart == 0) {
                    if (client->receiveBuffer[i] != ' ') websocketKeyStart = i;
                } else {
                    if (client->receiveBuffer[i] == ' ') break;
                }
            }
            websocketKeyLength = i - websocketKeyStart;
            if (websocketKeyLength != 24) websocketKeyStart = 0;
        }
        currentLine = lineEnd + 2;
    }

    if (isGet) {
        uint8_t *urlStart = &client->receiveBuffer[server_GET_SLASH_LEN];
        int32_t urlLength = 0;
        for (;;) {
            if (urlLength >= (firstLineEnd - server_GET_SLASH_LEN)) break;
            if (urlStart[urlLength] == ' ') break;
            ++urlLength;
        }

        if (websocketKeyStart != 0) {
            // Respond to websocket upgrade request.
            nolibc_MEMCPY(&self->scratchSpace[0], &client->receiveBuffer[websocketKeyStart], 24);
            nolibc_MEMCPY(&self->scratchSpace[24], "258EAFA5-E914-47DA-95CA-C5AB0DC85B11", 36);

            if (nolibc_write(self->sha1InstanceFd, &self->scratchSpace[0], 60) != 60) return -1;
            if (nolibc_read(self->sha1InstanceFd, &self->scratchSpace[256], 20) != 20) return -2;

            nolibc_MEMCPY(&self->scratchSpace[0], server_WEBSOCKET_ACCEPT_START, server_WEBSOCKET_ACCEPT_START_LEN);
            int32_t base64Len = base64_encode(&self->scratchSpace[256], 20, &self->scratchSpace[server_WEBSOCKET_ACCEPT_START_LEN]);
            nolibc_MEMCPY(&self->scratchSpace[server_WEBSOCKET_ACCEPT_START_LEN + base64Len], "\r\n\r\n", 4);

            client->receiveLength = 0;

            int32_t len = server_WEBSOCKET_ACCEPT_START_LEN + base64Len + 4;
            if (nolibc_sendto(client->fd, self->scratchSpace, len, MSG_NOSIGNAL, NULL, 0) != len) return -3;
            if (self->callbacks.onConnect(self->callbacks.data, client) != 0) return -4;
            // Only set this if the callback accepts the new connection.
            client->isWebsocket = true;
            return 1;
        }

        // Respond to normal GET request.
        for (int32_t i = 0; i < self->fileResponsesLength; ++i) {
            if (
                self->fileResponses[i].urlLength == urlLength &&
                nolibc_MEMCMP(self->fileResponses[i].url, urlStart, (uint64_t)urlLength) == 0
            ) {
                if (nolibc_sendto(client->fd, self->fileResponses[i].response, self->fileResponses[i].responseLength, MSG_NOSIGNAL, NULL, 0) != self->fileResponses[i].responseLength) return -3;
                return 0;
            }
        }
    }
    // We don't have any useful response, just send 404.
    if (nolibc_sendto(client->fd, "HTTP/1.1 404 Not Found\r\nContent-Length:0\r\n\r\n", 44, MSG_NOSIGNAL, NULL, 0) != 44) return -4;
    return 0;
}

static void server_closeClient(struct server *self, struct serverClient *client) {
    if (client->isWebsocket) self->callbacks.onDisconnect(self->callbacks.data, client);
    serverClient_close(client);
}

static int32_t server_handleClient(struct server *self, struct serverClient *client) {
    int32_t remainingBuffer = server_RECEIVE_BUFFER_SIZE - client->receiveLength;
    if (remainingBuffer <= 0) {
        server_closeClient(self, client);
        return -1;
    }

    uint8_t *receivePosition = &client->receiveBuffer[client->receiveLength];
    int32_t recvLength = (int32_t)nolibc_recvfrom(client->fd, receivePosition, remainingBuffer, 0, NULL, NULL);
    if (recvLength < 0) {
        server_closeClient(self, client);
        return -2;
    } else if (recvLength == 0) {
        server_closeClient(self, client);
        return 1;
    } else {
        client->receiveLength += recvLength;

        int32_t status;
        if (client->isWebsocket) status = server_handleWebsocket(self, client);
        else status = server_handleHttpRequest(self, client);

        if (status != 1) { // Connection no longer in progress.
            server_closeClient(self, client);
            if (status < 0) return -3;
        }
    }
    return 0;
}

static int32_t server_createTimer(struct server *self, int32_t *timerHandle) {
    int32_t fd = nolibc_timerfd_create(CLOCK_MONOTONIC, 0);
    if (fd < 0) return -1;

    struct epoll_event timerEvent = {
        .events = EPOLLIN,
        .data.ptr = timerHandle
    };
    if (nolibc_epoll_ctl(self->epollFd, EPOLL_CTL_ADD, fd, &timerEvent) < 0) return -2;

    *timerHandle = -fd; // Fd 0 is reserved for stdin, so this always gives a negative number.
    return 0;
}

static inline void server_startTimer(int32_t timerHandle, struct itimerspec *value, bool absolute) {
    uint32_t flags = absolute ? TFD_TIMER_ABSTIME : 0;
    nolibc_timerfd_settime(-timerHandle, flags, value, NULL);
}

static inline void server_stopTimer(int32_t timerHandle) {
    struct itimerspec value = {0};
    nolibc_timerfd_settime(-timerHandle, 0, &value, NULL);
}

static inline void server_destroyTimer(int32_t timerHandle) {
    nolibc_close(-timerHandle);
}

static int32_t server_run(struct server *self, bool busyWaiting) {
    int32_t timeout = busyWaiting ? 0 : -1;
    for (;;) {
        struct epoll_event event;
        int32_t status = nolibc_epoll_pwait(self->epollFd, &event, 1, timeout, NULL);
        if (status <= 0) continue;

        int32_t eventFd = *((int32_t *)event.data.ptr);
        if (eventFd < 0) {
            uint64_t expirations;
            if (nolibc_read(-eventFd, &expirations, 8) <= 0) return -1;
            self->callbacks.onTimer(self->callbacks.data, event.data.ptr, expirations);
        } else if (eventFd == self->listenSocketFd) {
            status = server_acceptSocket(self);
            if (status < 0) debug_printNum("Error accepting client socket! (", status, ")\n");
        } else {
            struct serverClient *client = (struct serverClient *)event.data.ptr;
            status = server_handleClient(self, client);
            if (status < 0) debug_printNum("Error handling client! (", status, ")\n");
        }
    }
    return 0;
}
