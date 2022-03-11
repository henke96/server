#define SELF ((struct game *)(self))

static int32_t game_onConnect(void *self, struct serverClient *serverClient) {
    // Complete handshake.
    uint32_t version = protocol_VERSION;
    server_sendWebsocketMessage(&SELF->server, serverClient, (uint8_t *)&version, 4);

    struct client *client = &SELF->clients[serverClient->index];
    client_create(client, serverClient);

    uint8_t view = protocol_GAME;
    server_sendWebsocketMessage(&SELF->server, client->serverClient, &view, 1);

    return 0;
}

static void game_onDisconnect(hc_UNUSED void *self, hc_UNUSED struct serverClient *serverClient) {}

static void game_onMessage(hc_UNUSED void *self, hc_UNUSED struct serverClient *serverClient, hc_UNUSED uint8_t *message, hc_UNUSED int32_t messageLength) {
}

static void game_onTimer(hc_UNUSED void *self, hc_UNUSED int32_t *timerHandle, hc_UNUSED uint64_t expirations) {}

#undef SELF

static int32_t game_init(struct game *self) {
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
        game_onConnect,
        game_onDisconnect,
        game_onMessage,
        game_onTimer
    );

    int32_t status = server_init(&self->server, 8089, &self->response, 1, &callbacks);
    if (status < 0) {
        debug_printNum("Server init failed! (", status, ")\n");
        return -1;
    }
    return 0;
}

static void game_deinit(struct game *self) {
    server_deinit(&self->server);
}

static int32_t game_run(struct game *self) {
    return server_run(&self->server, false);
}
