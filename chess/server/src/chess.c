static void chess_sendClientState(struct chess *self, struct chessClient *chessClient) {
    static uint8_t buffer[chessClient_writeState_MAX];
    int32_t length = chessClient_writeState(chessClient, &buffer[0]);
    server_sendWebsocketMessage(&self->server, chessClient->serverClient, &buffer[0], length);
}

static int32_t chess_createRoom(struct chess *self, struct chessClient *chessClient) {
    struct chessRoom *room = &self->rooms[0];
    // Atleast one room is guaranteed to be empty.
    for (;; ++room) {
        debug_ASSERT(room < &self->rooms[server_MAX_CLIENTS]);
        if (!chessRoom_isOpen(room)) break;
    }
    int32_t randomPart;
    hc_getrandom(&randomPart, 4, GRND_INSECURE);
    // Note: Relies on server_MAX_CLIENTS being power of 2!
    randomPart &= ~(server_MAX_CLIENTS - 1);
    randomPart &= 0x000FFFFF; // We don't need that much randomness.
    if (chessRoom_open(room, chessClient, randomPart | room->index, &self->server) < 0) return -1;
    chessClient_setRoom(chessClient, room);
    return 0;
}

static void chess_leaveRoom(struct chess *self, struct chessClient *chessClient) {
    struct chessRoom *room = chessClient->room;

    if (chessClient_isSpectator(chessClient)) {
        chessRoom_removeSpectator(room, chessClient);
        chessClient_unsetRoom(chessClient);
        return;
    }
    // Host or guest is leaving.
    while (room->numSpectators) {
        struct chessClient *spectator = chessRoom_spectators(room)[0];
        chessRoom_removeSpectator(room, spectator);
        chessClient_unsetRoom(spectator);
        chess_sendClientState(self, spectator);
    }

    if (chessRoom_isFull(room)) {
        struct chessClient *opponent = chessClient_isHost(chessClient) ? room->guest.client : room->host.client;
        chessClient_unsetRoom(opponent);
        chess_sendClientState(self, opponent);
    }
    chessClient_unsetRoom(chessClient);
    chessRoom_close(room);
}

static int32_t chess_handleCreate(struct chess *self, struct chessClient *chessClient) {
    if (chessClient_inRoom(chessClient)) return 0;
    if (chess_createRoom(self, chessClient) < 0) return 0; // Not client's fault.
    chess_sendClientState(self, chessClient);
    return 0;
}

static int32_t chess_handleJoin(struct chess *self, struct chessClient *chessClient, uint8_t *message, int32_t messageLength) {
    if (messageLength != 5) return -1;
    if (chessClient_inRoom(chessClient)) return 0;
    int32_t roomId;
    hc_MEMCPY(&roomId, &message[1], 4);

    struct chessRoom *room = &self->rooms[0];
    struct chessRoom *roomsEnd = &self->rooms[server_MAX_CLIENTS];
    for (; room != roomsEnd; ++room) {
        if (
            chessRoom_isOpen(room) &&
            room->roomId == roomId
        ) goto found;
    }
    return 0; // Doesn't exist.
    found:
    if (chessRoom_isFull(room)) return 0; // Already full.

    chessRoom_start(room, chessClient);
    chessClient_setRoom(chessClient, room);

    chess_sendClientState(self, chessClient);
    chess_sendClientState(self, room->host.client);

    for (int32_t i = 0; i < room->numSpectators; ++i) {
        struct chessClient *spectator = chessRoom_spectators(room)[i];
        chess_sendClientState(self, spectator);
    }
    return 0;
}

static int32_t chess_handleSpectate(struct chess *self, struct chessClient *chessClient, uint8_t *message, int32_t messageLength) {
    if (messageLength != 5) return -1;
    if (chessClient_inRoom(chessClient)) return 0;
    int32_t roomId;
    hc_MEMCPY(&roomId, &message[1], 4);

    struct chessRoom *room = &self->rooms[0];
    struct chessRoom *roomsEnd = &self->rooms[server_MAX_CLIENTS];
    for (; room != roomsEnd; ++room) {
        if (
            chessRoom_isOpen(room) &&
            room->roomId == roomId
        ) goto found;
    }
    return 0; // Doesn't exist.
    found:

    if (chessRoom_addSpectator(room, chessClient) < 0) return 0;
    chessClient_setRoom(chessClient, room);

    chess_sendClientState(self, chessClient);
    return 0;
}

static int32_t chess_handleMove(struct chess *self, struct chessClient *chessClient, uint8_t *message, int32_t messageLength) {
    if (messageLength != 3) return -1;
    if (!chessClient_inRoom(chessClient)) return 0;
    if (chessClient_isSpectator(chessClient)) return 0;

    struct chessRoom *room = chessClient->room;
    if (!chessRoom_isFull(room)) return 0;

    // Don't let player move unless they are viewing last move.
    if (chessClient->move != room->numMoves) return 0;

    bool isHost = chessClient_isHost(chessClient);
    if (isHost != chessRoom_isHostsTurn(chessClient->room)) return 0; // Not players turn.

    int32_t fromIndex = message[1];
    int32_t toIndex = message[2];

    if (chessRoom_isMoveValid(room, fromIndex, toIndex, isHost)) {
        chessRoom_doMove(room, fromIndex, toIndex, isHost);

        struct chessClient *opponent = isHost ? room->guest.client : room->host.client;
        chess_sendClientState(self, chessClient);
        chess_sendClientState(self, opponent);

        for (int32_t i = 0; i < room->numSpectators; ++i) {
            struct chessClient *spectator = chessRoom_spectators(room)[i];
            chess_sendClientState(self, spectator);
        }
    }
    return 0;
}

static int32_t chess_handleBack(struct chess *self, struct chessClient *chessClient) {
    if (!chessClient_inRoom(chessClient)) return 0;
    chess_leaveRoom(self, chessClient);
    chess_sendClientState(self, chessClient);
    return 0;
}

static int32_t chess_handleScroll(struct chess *self, struct chessClient *chessClient, uint8_t *message, int32_t messageLength) {
    if (messageLength != 2) return -1;
    if (!chessClient_inRoom(chessClient)) return 0;
    struct chessRoom *room = chessClient->room;
    if (!chessRoom_isFull(room)) return 0;
    uint8_t up = message[1];
    if (chessClient_scrollMove(chessClient, !up) == 0) {
        chess_sendClientState(self, chessClient);
    }
    return 0;
}

#define SELF ((struct chess *)(self))

static int32_t chess_onConnect(void *self, struct serverClient *client) {
    // Complete handshake.
    uint32_t version = protocol_VERSION;
    server_sendWebsocketMessage(&SELF->server, client, (uint8_t *)&version, 4);

    struct chessClient *chessClient = &SELF->clients[client->index];
    chessClient_create(chessClient, client);
    chess_sendClientState(SELF, chessClient);
    return 0;
}

static void chess_onDisconnect(void *self, struct serverClient *client) {
    struct chessClient *chessClient = &SELF->clients[client->index];
    if (chessClient_inRoom(chessClient)) {
        chess_leaveRoom(self, chessClient);
    }
}

static void chess_onMessage(void *self, struct serverClient *client, uint8_t *message, int32_t messageLength) {
    int32_t status = -1;
    if (messageLength >= 1) {
        struct chessClient *chessClient = &SELF->clients[client->index];
        switch (message[0]) {
            case protocol_CREATE: {
                status = chess_handleCreate(SELF, chessClient);
                if (status < 0) debug_printNum("Error creating room: ", status, "\n");
                break;
            }
            case protocol_JOIN: {
                status = chess_handleJoin(SELF, chessClient, message, messageLength);
                if (status < 0) debug_printNum("Error joining room: ", status, "\n");
                break;
            }
            case protocol_MOVE: {
                status = chess_handleMove(SELF, chessClient, message, messageLength);
                if (status < 0) debug_printNum("Error moving piece: ", status, "\n");
                break;
            }
            case protocol_BACK: {
                status = chess_handleBack(SELF, chessClient);
                if (status < 0) debug_printNum("Error going back: ", status, "\n");
                break;
            }
            case protocol_SPECTATE: {
                status = chess_handleSpectate(SELF, chessClient, message, messageLength);
                if (status < 0) debug_printNum("Error joining room as spectator: ", status, "\n");
                break;
            }
            case protocol_SCROLL: {
                status = chess_handleScroll(SELF, chessClient, message, messageLength);
                if (status < 0) debug_printNum("Error scrolling: ", status, "\n");
                break;
            }
        }
    }

    if (status < 0) {
        server_markForDisconnect(&SELF->server, client);
    }
}

static void chess_onTimer(void *self, int32_t *timerHandle, hc_UNUSED uint64_t expirations) {
    struct timespec currentTimespec;
    hc_clock_gettime(CLOCK_MONOTONIC, &currentTimespec);

    struct chessRoom *room = &SELF->rooms[0];
    for (;; ++room) {
        debug_ASSERT(room < &SELF->rooms[server_MAX_CLIENTS]);
        if (timerHandle == &room->secondTimerHandle) break;
    }
    int64_t currentTime = timespec_toNanoseconds(currentTimespec);
    chessRoom_updateTimeSpent(room, currentTime);

    chess_sendClientState(SELF, room->host.client);
    chess_sendClientState(SELF, room->guest.client);

    for (int32_t i = 0; i < room->numSpectators; ++i) {
        struct chessClient *spectator = chessRoom_spectators(room)[i];
        chess_sendClientState(SELF, spectator);
    }
}

#undef SELF

static int32_t chess_init(struct chess *self) {
    for (int32_t i = 0; i < server_MAX_CLIENTS; ++i) {
        chessRoom_create(&self->rooms[i], i);
    }

    fileResponse_create(
        &self->response,
        (uint8_t *)"",
        0,
        generatedHtml,
        sizeof(generatedHtml)
    );

    struct serverCallbacks callbacks;
    serverCallbacks_create(
        &callbacks,
        self,
        chess_onConnect,
        chess_onDisconnect,
        chess_onMessage,
        chess_onTimer
    );

    int32_t status = server_init(&self->server, 8089, &self->response, 1, &callbacks);
    if (status < 0) {
        debug_printNum("Server init failed! (", status, ")\n");
        return -1;
    }
    return 0;
}

static void chess_deinit(struct chess *self) {
    server_deinit(&self->server);
}

static int32_t chess_run(struct chess *self) {
    return server_run(&self->server, false);
}
