#define zobrist_GET(COLOUR, SQUARE, PIECE) zobrist_##COLOUR[(SQUARE) * NUM_PIECES + (PIECE)]

static uint64_t zobrist_white[64 * NUM_PIECES];
static uint64_t zobrist_black[64 * NUM_PIECES];
static uint64_t zobrist_move;

// xorshift64* by Sebastiano Vigna.
static uint64_t zobrist_rand(void) {
    static uint64_t x = 1070372;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    return x * 2685821657736338717LL;
}

static void zobrist_init(void) {
    zobrist_move = zobrist_rand();
    for (int32_t square = 0; square < 64; ++square) {
        for (int32_t piece = PAWN; piece < NUM_PIECES; ++piece) {
            zobrist_white[square * NUM_PIECES + piece] = zobrist_rand();
            zobrist_black[square * NUM_PIECES + piece] = zobrist_rand();
        }
    }
}