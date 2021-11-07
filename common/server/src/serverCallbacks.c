static inline void serverCallbacks_create(
    struct serverCallbacks *self,
    void *data,
    int32_t (*onConnect)(void *data, struct serverClient *client),
    void (*onDisconnect)(void *data, struct serverClient *client),
    int32_t (*onMessage)(void *data, struct serverClient *client, uint8_t *message, int32_t messageLength),
    void (*onTimer)(void *data, int32_t *timerHandle, uint64_t expirations)
) {
    self->data = data;
    self->onConnect = onConnect;
    self->onDisconnect = onDisconnect;
    self->onMessage = onMessage;
    self->onTimer = onTimer;
}
