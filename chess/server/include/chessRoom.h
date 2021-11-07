struct chessClient;
struct server;

struct chessRoom_clientInfo {
    struct chessClient *client; // NULL if none.
    int64_t lastUpdate;
    int64_t timeSpent;
};

struct chessRoom_move {
    int32_t fromIndex;
    int32_t toIndex;
    uint8_t piece;
    uint8_t replacePiece;
    int16_t __pad1;
};

struct chessRoom {
    int32_t index;
    int32_t roomId;
    uint8_t board[64];
    struct allocator spectators; // struct chessClient *
    int32_t numSpectators;
    int32_t numMoves;
    struct allocator moves; // struct chessRoom_move
    enum protocol_winner winner;
    int32_t secondTimerHandle;
    struct chessRoom_clientInfo host;
    struct chessRoom_clientInfo guest;
    bool hostsTurn;
    int8_t __pad1[7];
};

#define chessRoom_spectators(SELF) allocator_MEM(struct chessClient *, &(SELF)->spectators)
#define chessRoom_moves(SELF) allocator_MEM(struct chessRoom_move, &(SELF)->moves)

static inline void chessRoom_create(struct chessRoom *self, int32_t index);

static int32_t chessRoom_open(struct chessRoom *self, struct chessClient *host, int32_t roomId, struct server *server);
static void chessRoom_close(struct chessRoom *self);
static void chessRoom_start(struct chessRoom *self, struct chessClient *guest);
static int32_t chessRoom_addSpectator(struct chessRoom *self, struct chessClient *spectator);
static void chessRoom_removeSpectator(struct chessRoom *self, struct chessClient *spectator);

static inline bool chessRoom_isOpen(struct chessRoom *self);
static inline bool chessRoom_isFull(struct chessRoom *self);
static inline bool chessRoom_isHostsTurn(struct chessRoom *self);
static inline enum protocol_winner chessRoom_winner(struct chessRoom *self);

// `moveNumber` is between 0 to `numMoves` inclusive. The first move is 1, and { -1, -1, 0, 0 } is returned for move 0.
static struct chessRoom_move chessRoom_getMove(struct chessRoom *self, int32_t moveNumber, bool hostPov);
// Get board position after a certain `moveNumber` (see `chessRoom_getMove`).
static void chessRoom_getBoard(struct chessRoom *self, int32_t moveNumber, bool hostPov, uint8_t *outBoard);

static inline void chessRoom_updateTimeSpent(struct chessRoom *self, int64_t currentTime);
static inline int64_t chessRoom_timeSpent(struct chessRoom *self, bool hostPov);

// Validates coordinates.
static bool chessRoom_isMoveValid(struct chessRoom *self, int32_t fromIndex, int32_t toIndex, bool hostPov);

static void chessRoom_doMove(struct chessRoom *self, int32_t fromIndex, int32_t toIndex, bool hostPov);
