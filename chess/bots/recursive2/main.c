#include "../../../common/linux/hc/hc.h"
#include "../../../common/linux/hc/libc.h"
#include "../../../common/linux/hc/libc.c"
#include "../../../common/linux/hc/wrappers.c"
#include "../../../common/linux/debug.c"

#include "../../common/include/protocol.h"
#include "../common/client/include/client.h"

#include "../common/gen/gen.c"
#include "../common/asm.c"
#include "../common/client/src/client.c"

#define FILE_A ((uint64_t)0b0000000100000001000000010000000100000001000000010000000100000001U)
#define FILE_H ((uint64_t)0b1000000010000000100000001000000010000000100000001000000010000000U)
#define RANK2 ((uint64_t)0b0000000000000000000000000000000000000000000000001111111100000000U)
#define RANK7 ((uint64_t)0b0000000011111111000000000000000000000000000000000000000000000000U)
#define INFINITY INT32_MAX

static int32_t evaluateWhiteMoves(int32_t score, int32_t alpha, int32_t beta, int32_t remainingDepth);

struct move {
    int32_t from;
    int32_t to;
    int32_t score;
    bool exactScore;
    char __pad[3];
};

// Kept up to date with protocol.h
enum pieces {
    ALL = 0,
    NONE = 0,
    PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING,
    NUM_PIECES,
    PIECE_MASK = 0x3F,
    BLACK = 0x40,
    WHITE = 0x80
};
static_assert(
    NONE == (int)protocol_NO_PIECE && PAWN == (int)protocol_PAWN && BISHOP == (int)protocol_BISHOP &&
    KNIGHT == (int)protocol_KNIGHT && ROOK == (int)protocol_ROOK && QUEEN == (int)protocol_QUEEN && KING == (int)protocol_KING &&
    PIECE_MASK == (int)protocol_PIECE_MASK && BLACK == (int)protocol_BLACK_FLAG && WHITE == (int)protocol_WHITE_FLAG,
    "Not synced with protocol.h"
);

static struct move foundMoves[256];
static int32_t numFoundMoves;

static uint64_t white[NUM_PIECES];
static uint64_t black[NUM_PIECES];

static void init(bool isHost, uint8_t *board) {
    memset(&white, 0, sizeof(white));
    memset(&black, 0, sizeof(black));
    numFoundMoves = 0;

    for (int32_t i = 0; i < 64; ++i) {
        uint8_t piece = board[i];
        if (piece == NONE) continue;

        if (!isHost) {
            piece ^= (WHITE | BLACK);
            board[i] = piece;
        }

        uint64_t bit = (uint64_t)1 << i;
        if (piece & WHITE) {
            white[ALL] |= bit;
            switch (piece & PIECE_MASK) {
                case PAWN:   white[PAWN]   |= bit; break;
                case BISHOP: white[BISHOP] |= bit; break;
                case KNIGHT: white[KNIGHT] |= bit; break;
                case ROOK:   white[ROOK]   |= bit; break;
                case QUEEN:  white[QUEEN]  |= bit; break;
                case KING:   white[KING]   |= bit; break;
                default: hc_UNREACHABLE;
            }
        } else {
            black[ALL] |= bit;
            switch (piece & PIECE_MASK) {
                case PAWN:   black[PAWN]   |= bit; break;
                case BISHOP: black[BISHOP] |= bit; break;
                case KNIGHT: black[KNIGHT] |= bit; break;
                case ROOK:   black[ROOK]   |= bit; break;
                case QUEEN:  black[QUEEN]  |= bit; break;
                case KING:   black[KING]   |= bit; break;
                default: hc_UNREACHABLE;
            }
        }
    }
}

#define BISHOP_MOVES(FROM) gen_bishopMoves[((FROM) << 9) | asm_pext(white[ALL] | black[ALL], gen_bishopMasks[FROM])]
#define ROOK_MOVES(FROM) gen_rookMoves[((FROM) << 12) | asm_pext(white[ALL] | black[ALL], gen_rookMasks[FROM])]

#define EVALUATE_MOVE( \
    PIECE, \
    FROM_BIT, \
    TO_BIT, \
    PIECE_STATE, \
    MOVE_ACTION, \
    ... \
) \
PIECE_STATE[ALL] ^= (FROM_BIT | TO_BIT); \
PIECE_STATE[PIECE] ^= (FROM_BIT | TO_BIT); \
MOVE_ACTION \
PIECE_STATE[PIECE] ^= (FROM_BIT | TO_BIT); \
PIECE_STATE[ALL] ^= (FROM_BIT | TO_BIT); \
__VA_ARGS__

#define EVALUATE_CAPTURE_MOVE( \
    TARGET, \
    FROM_BIT, \
    TO_BIT, \
    PIECE, \
    PIECE_STATE, \
    OPP_PIECE_STATE, \
    MOVE_ACTION, \
    ... \
) \
OPP_PIECE_STATE[ALL] ^= TO_BIT; \
OPP_PIECE_STATE[TARGET] ^= TO_BIT; \
PIECE_STATE[ALL] ^= (FROM_BIT | TO_BIT); /* TODO: does this optimize well? */ \
PIECE_STATE[PIECE] ^= (FROM_BIT | TO_BIT); \
MOVE_ACTION \
PIECE_STATE[PIECE] ^= (FROM_BIT | TO_BIT); \
PIECE_STATE[ALL] ^= (FROM_BIT | TO_BIT); \
OPP_PIECE_STATE[TARGET] ^= TO_BIT; \
OPP_PIECE_STATE[ALL] ^= TO_BIT; \
__VA_ARGS__

#define EVALUATE_PAWN_PROMOTE_MOVE( \
    FROM_BIT, \
    TO_BIT, \
    PIECE_STATE, \
    MOVE_ACTION, \
    ... \
) \
PIECE_STATE[QUEEN] ^= TO_BIT; \
PIECE_STATE[ALL] ^= FROM_BIT; \
PIECE_STATE[PAWN] ^= (FROM_BIT | TO_BIT); \
MOVE_ACTION \
PIECE_STATE[PAWN] ^= (FROM_BIT | TO_BIT); \
PIECE_STATE[ALL] ^= FROM_BIT; \
PIECE_STATE[QUEEN] ^= TO_BIT; \
__VA_ARGS__

#define EVALUATE_PAWN_CAPTURE_PROMOTE_MOVE( \
    TARGET, \
    FROM_BIT, \
    TO_BIT, \
    PIECE_STATE, \
    OPP_PIECE_STATE, \
    MOVE_ACTION, \
    ... \
) \
OPP_PIECE_STATE[ALL] ^= TO_BIT; \
OPP_PIECE_STATE[TARGET] ^= TO_BIT; \
PIECE_STATE[QUEEN] ^= TO_BIT; \
PIECE_STATE[PAWN] ^= FROM_BIT; \
PIECE_STATE[ALL] ^= (FROM_BIT | TO_BIT); \
MOVE_ACTION \
PIECE_STATE[ALL] ^= (FROM_BIT | TO_BIT); \
PIECE_STATE[PAWN] ^= FROM_BIT; \
PIECE_STATE[QUEEN] ^= TO_BIT; \
OPP_PIECE_STATE[TARGET] ^= TO_BIT; \
OPP_PIECE_STATE[ALL] ^= TO_BIT; \
__VA_ARGS__

#define PAWN_CAPTURE_PROMOTE_MOVES( \
    TARGET, \
    PROMOTE_MASK, \
    MOVE_ACTION, \
    LEFT_FILE, \
    RIGHT_FILE, \
    SHIFT_UP_OP, \
    SHIFT_DOWN_OP, \
    PIECE_STATE, \
    OPP_PIECE_STATE, \
    ... \
) \
for (uint64_t PAWNS_LEFT = PIECE_STATE[PAWN] & PROMOTE_MASK & ~LEFT_FILE & (OPP_PIECE_STATE[TARGET] SHIFT_DOWN_OP 7); PAWNS_LEFT != 0; PAWNS_LEFT = asm_blsr(PAWNS_LEFT)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_LEFT); \
    uint64_t TO_BIT = (FROM_BIT SHIFT_UP_OP 7); \
    EVALUATE_PAWN_CAPTURE_PROMOTE_MOVE(TARGET, FROM_BIT, TO_BIT, PIECE_STATE, OPP_PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
} \
for (uint64_t PAWNS_RIGHT = PIECE_STATE[PAWN] & PROMOTE_MASK & ~RIGHT_FILE & (OPP_PIECE_STATE[TARGET] SHIFT_DOWN_OP 9); PAWNS_RIGHT != 0; PAWNS_RIGHT = asm_blsr(PAWNS_RIGHT)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_RIGHT); \
    uint64_t TO_BIT = (FROM_BIT SHIFT_UP_OP 9); \
    EVALUATE_PAWN_CAPTURE_PROMOTE_MOVE(TARGET, FROM_BIT, TO_BIT, PIECE_STATE, OPP_PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
}

#define CAPTURE_MOVES( \
    TARGET, \
    MOVE_ACTION, \
    LEFT_FILE, \
    RIGHT_FILE, \
    PROMOTE_MASK, \
    SHIFT_UP_OP, \
    SHIFT_DOWN_OP, \
    PIECE_STATE, \
    OPP_PIECE_STATE, \
    ... \
) \
for (uint64_t PAWNS_LEFT = PIECE_STATE[PAWN] & ~PROMOTE_MASK & ~LEFT_FILE & (OPP_PIECE_STATE[TARGET] SHIFT_DOWN_OP 7); PAWNS_LEFT != 0; PAWNS_LEFT = asm_blsr(PAWNS_LEFT)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_LEFT); \
    uint64_t TO_BIT = (FROM_BIT SHIFT_UP_OP 7); \
    EVALUATE_CAPTURE_MOVE(TARGET, FROM_BIT, TO_BIT, PAWN, PIECE_STATE, OPP_PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
} \
for (uint64_t PAWNS_RIGHT = PIECE_STATE[PAWN] & ~PROMOTE_MASK & ~RIGHT_FILE & (OPP_PIECE_STATE[TARGET] SHIFT_DOWN_OP 9); PAWNS_RIGHT != 0; PAWNS_RIGHT = asm_blsr(PAWNS_RIGHT)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_RIGHT); \
    uint64_t TO_BIT = (FROM_BIT SHIFT_UP_OP 9); \
    EVALUATE_CAPTURE_MOVE(TARGET, FROM_BIT, TO_BIT, PAWN, PIECE_STATE, OPP_PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
} \
for (uint64_t KNIGHTS = PIECE_STATE[KNIGHT]; KNIGHTS != 0; KNIGHTS = asm_blsr(KNIGHTS)) { \
    uint64_t FROM = asm_tzcnt(KNIGHTS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_knightMoves[FROM] & OPP_PIECE_STATE[TARGET]; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_CAPTURE_MOVE(TARGET, FROM_BIT, TO_BIT, KNIGHT, PIECE_STATE, OPP_PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
for (uint64_t BISHOPS = PIECE_STATE[BISHOP]; BISHOPS != 0; BISHOPS = asm_blsr(BISHOPS)) { \
    uint64_t FROM = asm_tzcnt(BISHOPS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = BISHOP_MOVES(FROM) & OPP_PIECE_STATE[TARGET]; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_CAPTURE_MOVE(TARGET, FROM_BIT, TO_BIT, BISHOP, PIECE_STATE, OPP_PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
for (uint64_t ROOKS = PIECE_STATE[ROOK]; ROOKS != 0; ROOKS = asm_blsr(ROOKS)) { \
    uint64_t FROM = asm_tzcnt(ROOKS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = ROOK_MOVES(FROM) & OPP_PIECE_STATE[TARGET]; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_CAPTURE_MOVE(TARGET, FROM_BIT, TO_BIT, ROOK, PIECE_STATE, OPP_PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
for (uint64_t QUEENS = PIECE_STATE[QUEEN]; QUEENS != 0; QUEENS = asm_blsr(QUEENS)) { \
    uint64_t FROM = asm_tzcnt(QUEENS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for ( \
        uint64_t MOVES = (BISHOP_MOVES(FROM) | ROOK_MOVES(FROM)) & OPP_PIECE_STATE[TARGET]; \
        MOVES != 0; \
        MOVES = asm_blsr(MOVES) \
    ) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_CAPTURE_MOVE(TARGET, FROM_BIT, TO_BIT, QUEEN, PIECE_STATE, OPP_PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
{ \
    uint64_t FROM = asm_tzcnt(PIECE_STATE[KING]); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_kingMoves[FROM] & OPP_PIECE_STATE[TARGET]; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_CAPTURE_MOVE(TARGET, FROM_BIT, TO_BIT, KING, PIECE_STATE, OPP_PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
}

#define EVALUATE_MOVES( \
    MOVE_ACTION, \
    SCORE_OP, \
    LEFT_FILE, \
    RIGHT_FILE, \
    PROMOTE_MASK, \
    SHIFT_UP_OP, \
    SHIFT_DOWN_OP, \
    PIECE_STATE, \
    OPP_PIECE_STATE, \
    DEPTH_CHECK, \
    ... \
) \
/* Generate all tiles that we attack (except for pawn promotions). */ \
uint64_t ATTACKED = ( \
    ((PIECE_STATE[PAWN] & ~PROMOTE_MASK & ~LEFT_FILE) SHIFT_UP_OP 7) | /* Left */ \
    ((PIECE_STATE[PAWN] & ~PROMOTE_MASK & ~RIGHT_FILE) SHIFT_UP_OP 9)  /* Right */ \
); \
ATTACKED |= gen_kingMoves[asm_tzcnt(PIECE_STATE[KING])]; \
ATTACKED |= gen_multiKnightMoves[(asm_lzcnt(PIECE_STATE[KNIGHT]) << 6) | asm_tzcnt(PIECE_STATE[KNIGHT])]; \
{ \
    uint64_t FROM = asm_tzcnt(PIECE_STATE[BISHOP]); \
    ATTACKED |= BISHOP_MOVES(FROM); \
    FROM = asm_tzcnt(asm_blsr(PIECE_STATE[BISHOP])); \
    ATTACKED |= BISHOP_MOVES(FROM); \
    FROM = asm_tzcnt(PIECE_STATE[ROOK]); \
    ATTACKED |= ROOK_MOVES(FROM); \
    FROM = asm_tzcnt(asm_blsr(PIECE_STATE[ROOK])); \
    ATTACKED |= ROOK_MOVES(FROM); \
    uint64_t QUEENS = PIECE_STATE[QUEEN]; \
    do { \
        FROM = asm_tzcnt(QUEENS); \
        ATTACKED |= (BISHOP_MOVES(FROM) | ROOK_MOVES(FROM)); \
        QUEENS = asm_blsr(QUEENS); \
    } while (QUEENS); \
} \
 \
if (ATTACKED & OPP_PIECE_STATE[KING]) return score SCORE_OP 10000; \
 \
int32_t best = SCORE_OP (-INFINITY); \
if (PIECE_STATE[PAWN] & PROMOTE_MASK) { \
    /* We have potential pawn promotions. */ \
    uint64_t PROMOTE_ATTACKED = ( \
        ((PIECE_STATE[PAWN] & PROMOTE_MASK & ~LEFT_FILE) SHIFT_UP_OP 7) | /* Left */ \
        ((PIECE_STATE[PAWN] & PROMOTE_MASK & ~RIGHT_FILE) SHIFT_UP_OP 9)  /* Right */ \
    ); \
    if (PROMOTE_ATTACKED & OPP_PIECE_STATE[KING]) return score SCORE_OP 10000; \
    /* Special case for depth 0 where we don't care which piece captures. */ \
    if (DEPTH_CHECK) { \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[QUEEN]) return score SCORE_OP (9 + 8); \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[ROOK]) return score SCORE_OP (5 + 8); \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[BISHOP]) return score SCORE_OP (3 + 8); \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[KNIGHT]) return score SCORE_OP (3 + 8); \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[PAWN]) return score SCORE_OP (1 + 8); \
    } else { \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[QUEEN]) { \
            score += SCORE_OP (9 + 8); \
            PAWN_CAPTURE_PROMOTE_MOVES(QUEEN, PROMOTE_MASK, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, SHIFT_UP_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
            score -= SCORE_OP (9 + 8); \
        } \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[ROOK]) { \
            score += SCORE_OP (5 + 8); \
            PAWN_CAPTURE_PROMOTE_MOVES(ROOK, PROMOTE_MASK, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, SHIFT_UP_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
            score -= SCORE_OP (5 + 8); \
        } \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[BISHOP]) { \
            score += SCORE_OP (3 + 8); \
            PAWN_CAPTURE_PROMOTE_MOVES(BISHOP, PROMOTE_MASK, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, SHIFT_UP_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
            score -= SCORE_OP (3 + 8); \
        } \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[KNIGHT]) { \
            score += SCORE_OP (3 + 8); \
            PAWN_CAPTURE_PROMOTE_MOVES(KNIGHT, PROMOTE_MASK, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, SHIFT_UP_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
            score -= SCORE_OP (3 + 8); \
        } \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[PAWN]) { \
            score += SCORE_OP (1 + 8); \
            PAWN_CAPTURE_PROMOTE_MOVES(PAWN, PROMOTE_MASK, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, SHIFT_UP_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
            score -= SCORE_OP (1 + 8); \
        } \
    } \
    { \
        uint64_t PAWN_PROMOTIONS = PIECE_STATE[PAWN] & PROMOTE_MASK & (~(white[ALL] | black[ALL]) SHIFT_DOWN_OP 8); \
        if (PAWN_PROMOTIONS) { \
            score += SCORE_OP 8; \
            if (DEPTH_CHECK) return score SCORE_OP !!(ATTACKED & OPP_PIECE_STATE[QUEEN]); /* 9 if we can capture a queen. */ \
            do { \
                uint64_t FROM_BIT = asm_blsi(PAWN_PROMOTIONS); \
                uint64_t TO_BIT = FROM_BIT SHIFT_UP_OP 8; \
                EVALUATE_PAWN_PROMOTE_MOVE(FROM_BIT, TO_BIT, PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
                PAWN_PROMOTIONS = asm_blsr(PAWN_PROMOTIONS); \
            } while (PAWN_PROMOTIONS); \
            score -= SCORE_OP 8; \
        } \
    } \
} \
/* Special case for depth 0 where we don't care which piece captures. */ \
if (DEPTH_CHECK) { \
    if (!(ATTACKED & OPP_PIECE_STATE[ALL])) return score; \
    if (ATTACKED & OPP_PIECE_STATE[QUEEN]) return score SCORE_OP 9; \
    if (ATTACKED & OPP_PIECE_STATE[ROOK]) return score SCORE_OP 5; \
    if (ATTACKED & OPP_PIECE_STATE[BISHOP]) return score SCORE_OP 3; \
    if (ATTACKED & OPP_PIECE_STATE[KNIGHT]) return score SCORE_OP 3; \
    return score SCORE_OP 1; /* Must be pawn. */ \
} \
if (ATTACKED & OPP_PIECE_STATE[QUEEN]) { \
    score += SCORE_OP 9; \
    CAPTURE_MOVES(QUEEN, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, PROMOTE_MASK, SHIFT_UP_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
    score -= SCORE_OP 9; \
} \
if (ATTACKED & OPP_PIECE_STATE[ROOK]) { \
    score += SCORE_OP 5; \
    CAPTURE_MOVES(ROOK, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, PROMOTE_MASK, SHIFT_UP_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
    score -= SCORE_OP 5; \
} \
if (ATTACKED & OPP_PIECE_STATE[BISHOP]) { \
    score += SCORE_OP 3; \
    CAPTURE_MOVES(BISHOP, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, PROMOTE_MASK, SHIFT_UP_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
    score -= SCORE_OP 3; \
} \
if (ATTACKED & OPP_PIECE_STATE[KNIGHT]) { \
    score += SCORE_OP 3; \
    CAPTURE_MOVES(KNIGHT, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, PROMOTE_MASK, SHIFT_UP_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
    score -= SCORE_OP 3; \
} \
if (ATTACKED & OPP_PIECE_STATE[PAWN]) { \
    score += SCORE_OP 1; \
    CAPTURE_MOVES(PAWN, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, PROMOTE_MASK, SHIFT_UP_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
    score -= SCORE_OP 1; \
} \
/* TODO: What move order for non captures? */ \
for (uint64_t KNIGHTS = PIECE_STATE[KNIGHT]; KNIGHTS != 0; KNIGHTS = asm_blsr(KNIGHTS)) { \
    uint64_t FROM = asm_tzcnt(KNIGHTS); \
    uint64_t MOVES = gen_knightMoves[FROM] & ~(white[ALL] | black[ALL]); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_MOVE(KNIGHT, FROM_BIT, TO_BIT, PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
for (uint64_t PAWNS_FORWARD = PIECE_STATE[PAWN] & ~PROMOTE_MASK & (~(white[ALL] | black[ALL]) SHIFT_DOWN_OP 8); PAWNS_FORWARD != 0; PAWNS_FORWARD = asm_blsr(PAWNS_FORWARD)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_FORWARD); \
    uint64_t TO_BIT = FROM_BIT SHIFT_UP_OP 8; \
    EVALUATE_MOVE(PAWN, FROM_BIT, TO_BIT, PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
} \
for (uint64_t BISHOPS = PIECE_STATE[BISHOP]; BISHOPS != 0; BISHOPS = asm_blsr(BISHOPS)) { \
    uint64_t FROM = asm_tzcnt(BISHOPS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = BISHOP_MOVES(FROM) & ~(white[ALL] | black[ALL]); MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_MOVE(BISHOP, FROM_BIT, TO_BIT, PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
for (uint64_t ROOKS = PIECE_STATE[ROOK]; ROOKS != 0; ROOKS = asm_blsr(ROOKS)) { \
    uint64_t FROM = asm_tzcnt(ROOKS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = ROOK_MOVES(FROM) & ~(white[ALL] | black[ALL]); MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_MOVE(ROOK, FROM_BIT, TO_BIT, PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
for (uint64_t QUEENS = PIECE_STATE[QUEEN]; QUEENS != 0; QUEENS = asm_blsr(QUEENS)) { \
    uint64_t FROM = asm_tzcnt(QUEENS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for ( \
        uint64_t MOVES = (BISHOP_MOVES(FROM) | ROOK_MOVES(FROM)) & ~(white[ALL] | black[ALL]); \
        MOVES != 0; \
        MOVES = asm_blsr(MOVES) \
    ) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_MOVE(QUEEN, FROM_BIT, TO_BIT, PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
{ \
    uint64_t FROM = asm_tzcnt(PIECE_STATE[KING]); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_kingMoves[FROM] & ~(white[ALL] | black[ALL]); MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_MOVE(KING, FROM_BIT, TO_BIT, PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
return best;

static int32_t evaluateBlackMoves(int32_t score, int32_t alpha, int32_t beta, int32_t remainingDepth) {
    EVALUATE_MOVES(
        int32_t SCORE = evaluateWhiteMoves(score, beta, alpha, remainingDepth);
        ,
        -, FILE_H, FILE_A, RANK2, >>, <<, black, white, false
        ,
        if (SCORE < best) {
            best = SCORE;
            if (best <= alpha) return best;
            if (best < beta) beta = best;
        }
    )
}

static int32_t evaluateWhiteMoves(int32_t score, int32_t beta, int32_t alpha, int32_t remainingDepth) {
    EVALUATE_MOVES(
        int32_t SCORE = evaluateBlackMoves(score, alpha, beta, remainingDepth - 1);
        ,
        +, FILE_A, FILE_H, RANK7, <<, >>, white, black, remainingDepth == 0
        ,
        if (SCORE > best) {
            best = SCORE;
            if (best >= beta) return best;
            if (best > alpha) alpha = best;
        }
    )
}

static int32_t countScore(void) {
    /*
    A faster version that assumes both kings alive:
    int32_t score = 6 * ((int32_t)asm_popcnt(white[QUEEN]) - (int32_t)asm_popcnt(black[QUEEN]));
    score += 2 * (
        ((int32_t)asm_popcnt(white[ROOK]) - (int32_t)asm_popcnt(white[PAWN])) -
        ((int32_t)asm_popcnt(black[ROOK]) - (int32_t)asm_popcnt(black[PAWN]))
    );
    score += 3 * ((int32_t)asm_popcnt(white[ALL]) - (int32_t)asm_popcnt(black[ALL]));
    return score;
    */

    int32_t score = 10000 * ((int32_t)asm_popcnt(white[KING]) - (int32_t)asm_popcnt(black[KING]));
    score += 9 * ((int32_t)asm_popcnt(white[QUEEN]) - (int32_t)asm_popcnt(black[QUEEN]));
    score += 5 * ((int32_t)asm_popcnt(white[ROOK]) - (int32_t)asm_popcnt(black[ROOK]));
    score += 3 * ((int32_t)asm_popcnt(white[BISHOP]) - (int32_t)asm_popcnt(black[BISHOP]));
    score += 3 * ((int32_t)asm_popcnt(white[KNIGHT]) - (int32_t)asm_popcnt(black[KNIGHT]));
    score += 1 * ((int32_t)asm_popcnt(white[PAWN]) - (int32_t)asm_popcnt(black[PAWN]));
    return score;
}

static void findMoves(uint8_t *board, int32_t initialScore) {
    for (uint64_t from = 0; from < 64; ++from) {
        uint8_t piece = board[from];
        if (!(piece & WHITE)) continue;

        piece &= PIECE_MASK;
        uint64_t fromBit = (uint64_t)1 << from;
        uint64_t moves;
        switch (piece) {
            case PAWN: {
                uint64_t left =  ((fromBit & ~FILE_A) << 7) & black[ALL];
                uint64_t right = ((fromBit & ~FILE_H) << 9) & black[ALL];
                uint64_t up = (fromBit << 8) & ~(white[ALL] | black[ALL]);
                moves = left | right | up;
                break;
            }
            case KNIGHT: moves = gen_knightMoves[from]                   & ~white[ALL]; break;
            case BISHOP: moves = BISHOP_MOVES(from)                      & ~white[ALL]; break;
            case ROOK:   moves = ROOK_MOVES(from)                        & ~white[ALL]; break;
            case QUEEN:  moves = (BISHOP_MOVES(from) | ROOK_MOVES(from)) & ~white[ALL]; break;
            case KING:   moves = gen_kingMoves[from]                     & ~white[ALL]; break;
            default: hc_UNREACHABLE;
        }

        for (; moves != 0; moves = asm_blsr(moves)) {
            uint64_t to = asm_tzcnt(moves);
            uint64_t toBit = (uint64_t)1 << to;
            uint64_t capture = board[to] & PIECE_MASK;

            int32_t absoluteScore;
            if (piece == PAWN && (fromBit & RANK7)) {
                if (capture != NONE) {
                    EVALUATE_PAWN_CAPTURE_PROMOTE_MOVE(
                        capture, fromBit, toBit, white, black,
                        absoluteScore = countScore();
                    )
                } else {
                    EVALUATE_PAWN_PROMOTE_MOVE(
                        fromBit, toBit, white,
                        absoluteScore = countScore();
                    )
                }
            } else {
                if (capture != NONE) {
                    EVALUATE_CAPTURE_MOVE(
                        capture, fromBit, toBit, piece, white, black,
                        absoluteScore = countScore();
                    )
                } else {
                    EVALUATE_MOVE(
                        piece, fromBit, toBit, white,
                        absoluteScore = countScore();
                    )
                }
            }
            int32_t score = absoluteScore - initialScore;
            foundMoves[numFoundMoves++] = (struct move) {
                .from = (int32_t)from,
                .to = (int32_t)to,
                .score = score,
                .exactScore = true
            };
        }
    }
}

static void sortMoves(void) {
    // Insertion sort.
    for (int32_t i = 1; i < numFoundMoves; ++i) {
        for (int32_t j = i; j > 0; --j) {
            struct move *other = &foundMoves[j - 1];
            if (
                (foundMoves[j].exactScore != other->exactScore && foundMoves[j].exactScore) ||
                (foundMoves[j].exactScore == other->exactScore && foundMoves[j].score > other->score)
            ) {
                struct move temp = *other;
                *other = foundMoves[j];
                foundMoves[j] = temp;
            }
        }
    }
}

static int32_t makeMove(bool isHost, uint8_t *board, hc_UNUSED int32_t lastMoveFrom, hc_UNUSED int32_t lastMoveTo, int32_t *moveFrom, int32_t *moveTo) {
    init(isHost, board);
    int32_t initialScore = countScore();
    findMoves(board, initialScore);
    int32_t targetDepth = 6; // Number of moves our side plays. Currently always end on our move.

    for (int32_t remainingDepth = 1; remainingDepth < targetDepth; ++remainingDepth) {
        sortMoves();

        int32_t alpha = -INFINITY;
        int32_t beta = INFINITY;

        if (remainingDepth >= 3) {
            alpha = foundMoves[0].score - 1;
            beta = foundMoves[0].score + 1;
        }

        for (;;) {
            debug_printNum("Search (remainingDepth=", remainingDepth, ", ");
            debug_printNum("alpha=", alpha, ", ");
            debug_printNum("beta=", beta, ")");
            int32_t best = alpha;
            for (int32_t i = 0; i < numFoundMoves; ++i) {
                struct move *move = &foundMoves[i];
                uint8_t piece = board[move->from] & PIECE_MASK;
                uint8_t capture = board[move->to] & PIECE_MASK;
                uint64_t fromBit = (uint64_t)1 << move->from;
                uint64_t toBit = (uint64_t)1 << move->to;

                if (toBit & black[KING]) {
                    // move->score already correct.
                } else if (piece == PAWN && (fromBit & RANK7)) {
                    if (capture != NONE) {
                        EVALUATE_PAWN_CAPTURE_PROMOTE_MOVE(
                            capture, fromBit, toBit, white, black,
                            move->score = evaluateBlackMoves(countScore() - initialScore, best, beta, remainingDepth - 1);
                        )
                    } else {
                        EVALUATE_PAWN_PROMOTE_MOVE(
                            fromBit, toBit, white,
                            move->score = evaluateBlackMoves(countScore() - initialScore, best, beta, remainingDepth - 1);
                        )
                    }
                } else {
                    if (capture != NONE) {
                        EVALUATE_CAPTURE_MOVE(
                            capture, fromBit, toBit, piece, white, black,
                            move->score = evaluateBlackMoves(countScore() - initialScore, best, beta, remainingDepth - 1);
                        )
                    } else {
                        EVALUATE_MOVE(
                            piece, fromBit, toBit, white,
                            move->score = evaluateBlackMoves(countScore() - initialScore, best, beta, remainingDepth - 1);
                        )
                    }
                }

                if (move->score > best) {
                    best = move->score;
                    if (best >= beta) break; // No point searching further, we missed the window.
                    move->exactScore = true;
                } else {
                    move->exactScore = false;
                }
            }
            debug_printNum(" = ", best, "\n");
            if (best <= alpha) {
                alpha = best - 1;
                beta = best + 1; // TODO: Keep when search becomes instable?
            } else if (best >= beta) {
                beta = best + 1;
                alpha = best - 1; // TODO: Keep when search becomes instable?
            } else break;
        }
    }

    sortMoves();

    for (int32_t i = numFoundMoves; i > 0;) {
        --i;
        if (foundMoves[i].exactScore) {
            debug_printNum("Exact move: ", foundMoves[i].from, "->");
            debug_printNum("", foundMoves[i].to, " ");
            debug_printNum("(", foundMoves[i].score, ")\n");
        } else {
            debug_printNum("Upperbound move: ", foundMoves[i].from, "->");
            debug_printNum("", foundMoves[i].to, " ");
            debug_printNum("(", foundMoves[i].score, ")\n");
        }
    }

    *moveFrom = foundMoves[0].from;
    *moveTo = foundMoves[0].to;
    return 0;
}

static struct client client;

int32_t main(int32_t argc, char **argv) {
    int32_t roomId = -1;
    if (argc > 1) {
        roomId = 0;
        for (char *s = argv[1]; *s != '\0'; ++s) {
            if (*s < '0' || *s > '9') return 1;
            roomId = 10 * roomId + (*s - '0');
        }
    }

    client_create(&client, makeMove);
    uint8_t address[] = { 127, 0, 0, 1 };
    int32_t status = client_run(&client, &address[0], 8089, roomId);
    debug_printNum("Status: ", status, "\n");
    return 0;
}