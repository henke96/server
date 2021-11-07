struct serverClient;

struct serverCallbacks {
    void *data;
    int32_t (*onConnect)(void *data, struct serverClient *client); // Non-zero return prevents the connection.
    void (*onDisconnect)(void *data, struct serverClient *client);
    int32_t (*onMessage)(void *data, struct serverClient *client, uint8_t *message, int32_t messageLength); // Non-zero return closes connection.
    void (*onTimer)(void *data, int32_t *timerHandle, uint64_t expirations); // `timerHandle` is the same pointer that was passed to `server_createTimer`.
                                                                         // `expirations` is number of expirations since last callback, at least 1.
};

static inline void serverCallbacks_create(
    struct serverCallbacks *self,
    void *data,
    int32_t (*onConnect)(void *data, struct serverClient *client),
    void (*onDisconnect)(void *data, struct serverClient *client),
    int32_t (*onMessage)(void *data, struct serverClient *client, uint8_t *message, int32_t messageLength),
    void (*onTimer)(void *data, int32_t *timerHandle, uint64_t expirations)
);
