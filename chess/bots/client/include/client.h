#define client_RECEIVE_BUFFER_SIZE 1024
#define client_SEND_BUFFER_SIZE 1024

typedef int32_t (*client_makeMove)(bool isHost, uint8_t *board, int32_t lastMoveFrom, int32_t lastMoveTo, int32_t *moveFrom, int32_t *moveTo);

struct client_gameState {
    bool wasHostsTurn;
    bool isHost;
};

struct client {
    int32_t socketFd;
    uint8_t receiveBuffer[client_RECEIVE_BUFFER_SIZE];
    uint8_t sendBuffer[client_SEND_BUFFER_SIZE];
    int32_t received;
    client_makeMove makeMove;
    struct client_gameState state;
    uint8_t __pad1[6];
};

static inline void client_create(struct client *self, client_makeMove makeMove);
static int32_t client_run(struct client *self, uint8_t *address, uint16_t port, int32_t roomId);
