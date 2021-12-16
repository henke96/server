#define common_MAX_MOVES 256
#define common_CONVERT_INDEX(INDEX) (INDEX + 26 + 4 * (INDEX / 8))
#define common_CONVERT_INDEX_BACK(INDEX) (INDEX - 18 - 4 * (INDEX / 12))

struct common_move {
    int32_t from;
    int32_t to;
};

// Transform board to 12x12 with 2 wide borders, and pretend we are always white.
static void common_transformBoard(bool isHost, uint8_t *board);
static void common_dumpBoard(void);
static void common_findMoves(void);
