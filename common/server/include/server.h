#define server_RECEIVE_BUFFER_SIZE 4096
#define server_MAX_CLIENTS 256

struct fileResponse;
struct itimerspec;

struct server {
    int32_t listenSocketFd;
    int32_t epollFd;
    int32_t sha1SocketFd;
    int32_t sha1InstanceFd;
    struct serverClient clients[server_MAX_CLIENTS];
    struct fileResponse *fileResponses;
    struct serverCallbacks callbacks;
    int32_t fileResponsesLength;
    int32_t clientDisconnectList; // Index of first list entry, -1 if empty.
    uint8_t scratchSpace[512];
};

static int32_t server_init(
    struct server *self,
    uint16_t port,
    struct fileResponse *fileResponses,
    int32_t fileResponsesLength,
    struct serverCallbacks *callbacks // Copied
);
static inline void server_deinit(struct server *self);
static int32_t server_run(struct server *self, bool busyWaiting);

// Mark a client to be disconnected.
static void server_markForDisconnect(struct server *self, struct serverClient *client);

static void server_sendWebsocketMessage(struct server *self, struct serverClient *client, uint8_t *message, int32_t messageLength);

// `*timerHandle` will be a negative number.
static int32_t server_createTimer(struct server *self, int32_t *timerHandle);
// See timerfd_settime(), `new_value`.
static inline void server_startTimer(int32_t timerHandle, struct itimerspec *value, bool absolute);
static inline void server_stopTimer(int32_t timerHandle);
static inline void server_destroyTimer(int32_t timerHandle);
