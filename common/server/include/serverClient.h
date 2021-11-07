#define serverClient_RECEIVE_BUFFER_SIZE 4096

struct serverClient {
    int32_t fd;
    uint8_t receiveBuffer[serverClient_RECEIVE_BUFFER_SIZE];
    int32_t receiveLength;
    int32_t index;
    bool isWebsocket;
    int8_t __pad1[3];
};

static inline void serverClient_init(struct serverClient *self, int32_t index);
static inline void serverClient_deinit(struct serverClient *self);
static inline void serverClient_open(struct serverClient *self, int32_t fd);
static inline void serverClient_close(struct serverClient *self);
