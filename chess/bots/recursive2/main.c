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

#include "main.h"
#include "tt.c"
#include "zobrist.c"

struct move {
    int32_t from;
    int32_t to;
    int32_t score;
    bool exactScore;
    char __pad[3];
};

static struct move foundMoves[256];
static int32_t numFoundMoves;

static uint64_t white[NUM_PIECES];
static uint64_t black[NUM_PIECES];

hc_UNUSED
static void assertZobrist(uint64_t zobrist, const char *line) {
    uint64_t actualZobrist = 0;
    for (uint64_t piece = PAWN; piece < NUM_PIECES; ++piece) {
        for (uint64_t pieces = white[piece]; pieces != 0; pieces = asm_blsr(pieces)) {
            uint64_t pieceSquare = asm_tzcnt(pieces);
            actualZobrist ^= zobrist_GET(white, pieceSquare, piece);
        }
        for (uint64_t pieces = black[piece]; pieces != 0; pieces = asm_blsr(pieces)) {
            uint64_t pieceSquare = asm_tzcnt(pieces);
            actualZobrist ^= zobrist_GET(black, pieceSquare, piece);
        }
    }
    if (zobrist != actualZobrist) {
        debug_printNum(line, 0, "\n");
        hc_exit(0);
    }
}

static void init(bool isHost, uint8_t *board, uint64_t *zobrist) {
    zobrist_init();
    memset(&white, 0, sizeof(white));
    memset(&black, 0, sizeof(black));
    numFoundMoves = 0;
    *zobrist = 0;

    for (int32_t square = 0; square < 64; ++square) {
        uint8_t piece = board[square];
        if (piece == NONE) continue;

        if (!isHost) {
            piece ^= (WHITE | BLACK);
            board[square] = piece;
        }

        uint64_t bit = (uint64_t)1 << square;
        if (piece & WHITE) {
            white[ALL] |= bit;
            *zobrist ^= zobrist_GET(white, square, piece & PIECE_MASK);
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
            *zobrist ^= zobrist_GET(black, square, piece & PIECE_MASK);
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
    ZOBRIST, \
    TO, \
    COLOUR, \
    MOVE_ACTION, \
    ... \
) \
uint64_t TO_FROM_BITS = FROM_BIT | ((uint64_t)1 << TO); \
COLOUR[ALL] ^= TO_FROM_BITS; \
COLOUR[PIECE] ^= TO_FROM_BITS; \
hc_UNUSED uint64_t NEW_ZOBRIST = ZOBRIST ^ zobrist_GET(COLOUR, TO, PIECE); \
MOVE_ACTION \
COLOUR[PIECE] ^= TO_FROM_BITS; \
COLOUR[ALL] ^= TO_FROM_BITS; \
__VA_ARGS__

#define EVALUATE_CAPTURE_MOVE( \
    TARGET, \
    FROM_BIT, \
    ZOBRIST, \
    TO, \
    PIECE, \
    COLOUR, \
    OPP_COLOUR, \
    MOVE_ACTION, \
    ... \
) \
uint64_t TO_BIT = (uint64_t)1 << TO; \
OPP_COLOUR[ALL] ^= TO_BIT; \
OPP_COLOUR[TARGET] ^= TO_BIT; \
COLOUR[ALL] ^= (FROM_BIT | TO_BIT); \
COLOUR[PIECE] ^= (FROM_BIT | TO_BIT); \
hc_UNUSED uint64_t NEW_ZOBRIST = ZOBRIST ^ zobrist_GET(COLOUR, TO, PIECE) ^ zobrist_GET(OPP_COLOUR, TO, TARGET); \
MOVE_ACTION \
COLOUR[PIECE] ^= (FROM_BIT | TO_BIT); \
COLOUR[ALL] ^= (FROM_BIT | TO_BIT); \
OPP_COLOUR[TARGET] ^= TO_BIT; \
OPP_COLOUR[ALL] ^= TO_BIT; \
__VA_ARGS__

#define EVALUATE_PAWN_PROMOTE_MOVE( \
    FROM_BIT, \
    ZOBRIST, \
    TO, \
    COLOUR, \
    MOVE_ACTION, \
    ... \
) \
uint64_t TO_BIT = ((uint64_t)1 << TO); \
COLOUR[QUEEN] ^= TO_BIT; \
COLOUR[PAWN] ^= FROM_BIT; \
COLOUR[ALL] ^= (FROM_BIT | TO_BIT); \
hc_UNUSED uint64_t NEW_ZOBRIST = ZOBRIST ^ zobrist_GET(COLOUR, TO, QUEEN); \
MOVE_ACTION \
COLOUR[ALL] ^= (FROM_BIT | TO_BIT); \
COLOUR[PAWN] ^= FROM_BIT; \
COLOUR[QUEEN] ^= TO_BIT; \
__VA_ARGS__

#define EVALUATE_PAWN_CAPTURE_PROMOTE_MOVE( \
    TARGET, \
    FROM_BIT, \
    ZOBRIST, \
    TO, \
    COLOUR, \
    OPP_COLOUR, \
    MOVE_ACTION, \
    ... \
) \
uint64_t TO_BIT = (uint64_t)1 << TO; \
OPP_COLOUR[ALL] ^= TO_BIT; \
OPP_COLOUR[TARGET] ^= TO_BIT; \
COLOUR[QUEEN] ^= TO_BIT; \
COLOUR[PAWN] ^= FROM_BIT; \
COLOUR[ALL] ^= (FROM_BIT | TO_BIT); \
hc_UNUSED uint64_t NEW_ZOBRIST = ZOBRIST ^ zobrist_GET(COLOUR, TO, QUEEN) ^ zobrist_GET(OPP_COLOUR, TO, TARGET); \
MOVE_ACTION \
COLOUR[ALL] ^= (FROM_BIT | TO_BIT); \
COLOUR[PAWN] ^= FROM_BIT; \
COLOUR[QUEEN] ^= TO_BIT; \
OPP_COLOUR[TARGET] ^= TO_BIT; \
OPP_COLOUR[ALL] ^= TO_BIT; \
__VA_ARGS__

#define PAWN_CAPTURE_PROMOTE_MOVES( \
    TARGET, \
    PROMOTE_MASK, \
    MOVE_ACTION, \
    LEFT_FILE, \
    RIGHT_FILE, \
    ADD_OP, \
    SHIFT_DOWN_OP, \
    COLOUR, \
    OPP_COLOUR, \
    ... \
) \
for (uint64_t PAWNS_LEFT = COLOUR[PAWN] & PROMOTE_MASK & ~LEFT_FILE & (OPP_COLOUR[TARGET] SHIFT_DOWN_OP 7); PAWNS_LEFT != 0; PAWNS_LEFT = asm_blsr(PAWNS_LEFT)) { \
    uint64_t FROM = asm_tzcnt(PAWNS_LEFT); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    uint64_t ZOBRIST = zobrist ^ zobrist_GET(COLOUR, FROM, PAWN); \
    uint64_t TO = FROM ADD_OP 7; \
    EVALUATE_PAWN_CAPTURE_PROMOTE_MOVE(TARGET, FROM_BIT, ZOBRIST, TO, COLOUR, OPP_COLOUR, MOVE_ACTION, __VA_ARGS__) \
} \
for (uint64_t PAWNS_RIGHT = COLOUR[PAWN] & PROMOTE_MASK & ~RIGHT_FILE & (OPP_COLOUR[TARGET] SHIFT_DOWN_OP 9); PAWNS_RIGHT != 0; PAWNS_RIGHT = asm_blsr(PAWNS_RIGHT)) { \
    uint64_t FROM = asm_tzcnt(PAWNS_RIGHT); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    uint64_t ZOBRIST = zobrist ^ zobrist_GET(COLOUR, FROM, PAWN); \
    uint64_t TO = FROM ADD_OP 9; \
    EVALUATE_PAWN_CAPTURE_PROMOTE_MOVE(TARGET, FROM_BIT, ZOBRIST, TO, COLOUR, OPP_COLOUR, MOVE_ACTION, __VA_ARGS__) \
}

#define CAPTURE_MOVES( \
    TARGET, \
    MOVE_ACTION, \
    LEFT_FILE, \
    RIGHT_FILE, \
    PROMOTE_MASK, \
    ADD_OP, \
    SHIFT_DOWN_OP, \
    PIECE_STATE, \
    OPP_PIECE_STATE, \
    ... \
) \
for (uint64_t PAWNS_LEFT = PIECE_STATE[PAWN] & ~PROMOTE_MASK & ~LEFT_FILE & (OPP_PIECE_STATE[TARGET] SHIFT_DOWN_OP 7); PAWNS_LEFT != 0; PAWNS_LEFT = asm_blsr(PAWNS_LEFT)) { \
    uint64_t FROM = asm_tzcnt(PAWNS_LEFT); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    uint64_t ZOBRIST = zobrist ^ zobrist_GET(PIECE_STATE, FROM, PAWN); \
    uint64_t TO = FROM ADD_OP 7; \
    EVALUATE_CAPTURE_MOVE(TARGET, FROM_BIT, ZOBRIST, TO, PAWN, PIECE_STATE, OPP_PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
} \
for (uint64_t PAWNS_RIGHT = PIECE_STATE[PAWN] & ~PROMOTE_MASK & ~RIGHT_FILE & (OPP_PIECE_STATE[TARGET] SHIFT_DOWN_OP 9); PAWNS_RIGHT != 0; PAWNS_RIGHT = asm_blsr(PAWNS_RIGHT)) { \
    uint64_t FROM = asm_tzcnt(PAWNS_RIGHT); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    uint64_t ZOBRIST = zobrist ^ zobrist_GET(PIECE_STATE, FROM, PAWN); \
    uint64_t TO = FROM ADD_OP 9; \
    EVALUATE_CAPTURE_MOVE(TARGET, FROM_BIT, ZOBRIST, TO, PAWN, PIECE_STATE, OPP_PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
} \
for (uint64_t KNIGHTS = PIECE_STATE[KNIGHT]; KNIGHTS != 0; KNIGHTS = asm_blsr(KNIGHTS)) { \
    uint64_t FROM = asm_tzcnt(KNIGHTS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    uint64_t ZOBRIST = zobrist ^ zobrist_GET(PIECE_STATE, FROM, KNIGHT); \
    for (uint64_t MOVES = gen_knightMoves[FROM] & OPP_PIECE_STATE[TARGET]; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO = asm_tzcnt(MOVES); \
        EVALUATE_CAPTURE_MOVE(TARGET, FROM_BIT, ZOBRIST, TO, KNIGHT, PIECE_STATE, OPP_PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
for (uint64_t BISHOPS = PIECE_STATE[BISHOP]; BISHOPS != 0; BISHOPS = asm_blsr(BISHOPS)) { \
    uint64_t FROM = asm_tzcnt(BISHOPS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    uint64_t ZOBRIST = zobrist ^ zobrist_GET(PIECE_STATE, FROM, BISHOP); \
    for (uint64_t MOVES = BISHOP_MOVES(FROM) & OPP_PIECE_STATE[TARGET]; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO = asm_tzcnt(MOVES); \
        EVALUATE_CAPTURE_MOVE(TARGET, FROM_BIT, ZOBRIST, TO, BISHOP, PIECE_STATE, OPP_PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
for (uint64_t ROOKS = PIECE_STATE[ROOK]; ROOKS != 0; ROOKS = asm_blsr(ROOKS)) { \
    uint64_t FROM = asm_tzcnt(ROOKS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    uint64_t ZOBRIST = zobrist ^ zobrist_GET(PIECE_STATE, FROM, ROOK); \
    for (uint64_t MOVES = ROOK_MOVES(FROM) & OPP_PIECE_STATE[TARGET]; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO = asm_tzcnt(MOVES); \
        EVALUATE_CAPTURE_MOVE(TARGET, FROM_BIT, ZOBRIST, TO, ROOK, PIECE_STATE, OPP_PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
for (uint64_t QUEENS = PIECE_STATE[QUEEN]; QUEENS != 0; QUEENS = asm_blsr(QUEENS)) { \
    uint64_t FROM = asm_tzcnt(QUEENS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    uint64_t ZOBRIST = zobrist ^ zobrist_GET(PIECE_STATE, FROM, QUEEN); \
    for ( \
        uint64_t MOVES = (BISHOP_MOVES(FROM) | ROOK_MOVES(FROM)) & OPP_PIECE_STATE[TARGET]; \
        MOVES != 0; \
        MOVES = asm_blsr(MOVES) \
    ) { \
        uint64_t TO = asm_tzcnt(MOVES); \
        EVALUATE_CAPTURE_MOVE(TARGET, FROM_BIT, ZOBRIST, TO, QUEEN, PIECE_STATE, OPP_PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
{ \
    uint64_t FROM = asm_tzcnt(PIECE_STATE[KING]); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    uint64_t ZOBRIST = zobrist ^ zobrist_GET(PIECE_STATE, FROM, KING); \
    for (uint64_t MOVES = gen_kingMoves[FROM] & OPP_PIECE_STATE[TARGET]; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO = asm_tzcnt(MOVES); \
        EVALUATE_CAPTURE_MOVE(TARGET, FROM_BIT, ZOBRIST, TO, KING, PIECE_STATE, OPP_PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
}

#define EVALUATE_MOVES( \
    MOVE_ACTION, \
    ADD_OP, \
    LEFT_FILE, \
    RIGHT_FILE, \
    PROMOTE_MASK, \
    SHIFT_UP_OP, \
    SHIFT_DOWN_OP, \
    PIECE_STATE, \
    OPP_PIECE_STATE, \
    DEPTH_CHECK, \
    TT_CUTOFF_CHECK, \
    TT_ALL_CHECK, \
    ... \
) \
zobrist ^= zobrist_move; \
uint64_t TT_ENTRY = tt[tt_INDEX(zobrist)]; \
if ((uint32_t)TT_ENTRY == (zobrist >> 32)) { \
    if (remainingDepth <= (int32_t)tt_GET_DEPTH(TT_ENTRY)) { \
        int32_t TT_SCORE = tt_GET_SCORE(TT_ENTRY); \
        if (TT_ENTRY & tt_SCORE_TYPE_CUTOFF) { \
            TT_CUTOFF_CHECK \
        } else if (TT_ENTRY & tt_SCORE_TYPE_EXACT) { \
            return TT_SCORE; \
        } else { \
            TT_ALL_CHECK \
        } \
    } \
} \
TT_ENTRY = (zobrist >> 32); \
\
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
int32_t BEST = ADD_OP (-INFINITY); \
if (ATTACKED & OPP_PIECE_STATE[KING]) { \
    BEST = score ADD_OP 1000; \
    goto STOP_SEARCH; \
} \
if (PIECE_STATE[PAWN] & PROMOTE_MASK) { \
    /* We have potential pawn promotions. */ \
    uint64_t PROMOTE_ATTACKED = ( \
        ((PIECE_STATE[PAWN] & PROMOTE_MASK & ~LEFT_FILE) SHIFT_UP_OP 7) | /* Left */ \
        ((PIECE_STATE[PAWN] & PROMOTE_MASK & ~RIGHT_FILE) SHIFT_UP_OP 9)  /* Right */ \
    ); \
    if (PROMOTE_ATTACKED & OPP_PIECE_STATE[KING]) { \
        BEST = score ADD_OP 1000; \
        goto STOP_SEARCH; \
    } \
    /* Special case for depth 0 where we don't care which piece captures. */ \
    if (DEPTH_CHECK) { \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[QUEEN]) return score ADD_OP (9 + 8); \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[ROOK]) return score ADD_OP (5 + 8); \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[BISHOP]) return score ADD_OP (3 + 8); \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[KNIGHT]) return score ADD_OP (3 + 8); \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[PAWN]) return score ADD_OP (1 + 8); \
    } else { \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[QUEEN]) { \
            score += ADD_OP (9 + 8); \
            PAWN_CAPTURE_PROMOTE_MOVES(QUEEN, PROMOTE_MASK, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, ADD_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
            score -= ADD_OP (9 + 8); \
        } \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[ROOK]) { \
            score += ADD_OP (5 + 8); \
            PAWN_CAPTURE_PROMOTE_MOVES(ROOK, PROMOTE_MASK, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, ADD_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
            score -= ADD_OP (5 + 8); \
        } \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[BISHOP]) { \
            score += ADD_OP (3 + 8); \
            PAWN_CAPTURE_PROMOTE_MOVES(BISHOP, PROMOTE_MASK, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, ADD_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
            score -= ADD_OP (3 + 8); \
        } \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[KNIGHT]) { \
            score += ADD_OP (3 + 8); \
            PAWN_CAPTURE_PROMOTE_MOVES(KNIGHT, PROMOTE_MASK, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, ADD_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
            score -= ADD_OP (3 + 8); \
        } \
        if (PROMOTE_ATTACKED & OPP_PIECE_STATE[PAWN]) { \
            score += ADD_OP (1 + 8); \
            PAWN_CAPTURE_PROMOTE_MOVES(PAWN, PROMOTE_MASK, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, ADD_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
            score -= ADD_OP (1 + 8); \
        } \
    } \
    { \
        uint64_t PAWN_PROMOTIONS = PIECE_STATE[PAWN] & PROMOTE_MASK & (~(white[ALL] | black[ALL]) SHIFT_DOWN_OP 8); \
        if (PAWN_PROMOTIONS) { \
            score += ADD_OP 8; \
            if (DEPTH_CHECK) return score ADD_OP !!(ATTACKED & OPP_PIECE_STATE[QUEEN]); /* 9 if we can capture a queen. */ \
            do { \
                uint64_t FROM = asm_tzcnt(PAWN_PROMOTIONS); \
                uint64_t FROM_BIT = (uint64_t)1 << FROM; \
                uint64_t ZOBRIST = zobrist ^ zobrist_GET(PIECE_STATE, FROM, PAWN); \
                uint64_t TO = FROM ADD_OP 8; \
                EVALUATE_PAWN_PROMOTE_MOVE(FROM_BIT, ZOBRIST, TO, PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
                PAWN_PROMOTIONS = asm_blsr(PAWN_PROMOTIONS); \
            } while (PAWN_PROMOTIONS); \
            score -= ADD_OP 8; \
        } \
    } \
} \
/* Special case for depth 0 where we don't care which piece captures. */ \
if (DEPTH_CHECK) { \
    if (!(ATTACKED & OPP_PIECE_STATE[ALL])) return score; \
    if (ATTACKED & OPP_PIECE_STATE[QUEEN]) return score ADD_OP 9; \
    if (ATTACKED & OPP_PIECE_STATE[ROOK]) return score ADD_OP 5; \
    if (ATTACKED & OPP_PIECE_STATE[BISHOP]) return score ADD_OP 3; \
    if (ATTACKED & OPP_PIECE_STATE[KNIGHT]) return score ADD_OP 3; \
    return score ADD_OP 1; /* Must be pawn. */ \
} \
if (ATTACKED & OPP_PIECE_STATE[QUEEN]) { \
    score += ADD_OP 9; \
    CAPTURE_MOVES(QUEEN, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, PROMOTE_MASK, ADD_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
    score -= ADD_OP 9; \
} \
if (ATTACKED & OPP_PIECE_STATE[ROOK]) { \
    score += ADD_OP 5; \
    CAPTURE_MOVES(ROOK, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, PROMOTE_MASK, ADD_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
    score -= ADD_OP 5; \
} \
if (ATTACKED & OPP_PIECE_STATE[BISHOP]) { \
    score += ADD_OP 3; \
    CAPTURE_MOVES(BISHOP, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, PROMOTE_MASK, ADD_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
    score -= ADD_OP 3; \
} \
if (ATTACKED & OPP_PIECE_STATE[KNIGHT]) { \
    score += ADD_OP 3; \
    CAPTURE_MOVES(KNIGHT, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, PROMOTE_MASK, ADD_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
    score -= ADD_OP 3; \
} \
if (ATTACKED & OPP_PIECE_STATE[PAWN]) { \
    score += ADD_OP 1; \
    CAPTURE_MOVES(PAWN, MOVE_ACTION, LEFT_FILE, RIGHT_FILE, PROMOTE_MASK, ADD_OP, SHIFT_DOWN_OP, PIECE_STATE, OPP_PIECE_STATE, __VA_ARGS__) \
    score -= ADD_OP 1; \
} \
/* TODO: What move order for non captures? */ \
for (uint64_t KNIGHTS = PIECE_STATE[KNIGHT]; KNIGHTS != 0; KNIGHTS = asm_blsr(KNIGHTS)) { \
    uint64_t FROM = asm_tzcnt(KNIGHTS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    uint64_t ZOBRIST = zobrist ^ zobrist_GET(PIECE_STATE, FROM, KNIGHT); \
    for (uint64_t MOVES = gen_knightMoves[FROM] & ~(white[ALL] | black[ALL]); MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO = asm_tzcnt(MOVES); \
        EVALUATE_MOVE(KNIGHT, FROM_BIT, ZOBRIST, TO, PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
for (uint64_t PAWNS_FORWARD = PIECE_STATE[PAWN] & ~PROMOTE_MASK & (~(white[ALL] | black[ALL]) SHIFT_DOWN_OP 8); PAWNS_FORWARD != 0; PAWNS_FORWARD = asm_blsr(PAWNS_FORWARD)) { \
    uint64_t FROM = asm_tzcnt(PAWNS_FORWARD); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    uint64_t ZOBRIST = zobrist ^ zobrist_GET(PIECE_STATE, FROM, PAWN); \
    uint64_t TO = FROM ADD_OP 8; \
    EVALUATE_MOVE(PAWN, FROM_BIT, ZOBRIST, TO, PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
} \
for (uint64_t BISHOPS = PIECE_STATE[BISHOP]; BISHOPS != 0; BISHOPS = asm_blsr(BISHOPS)) { \
    uint64_t FROM = asm_tzcnt(BISHOPS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    uint64_t ZOBRIST = zobrist ^ zobrist_GET(PIECE_STATE, FROM, BISHOP); \
    for (uint64_t MOVES = BISHOP_MOVES(FROM) & ~(white[ALL] | black[ALL]); MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO = asm_tzcnt(MOVES); \
        EVALUATE_MOVE(BISHOP, FROM_BIT, ZOBRIST, TO, PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
for (uint64_t ROOKS = PIECE_STATE[ROOK]; ROOKS != 0; ROOKS = asm_blsr(ROOKS)) { \
    uint64_t FROM = asm_tzcnt(ROOKS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    uint64_t ZOBRIST = zobrist ^ zobrist_GET(PIECE_STATE, FROM, ROOK); \
    for (uint64_t MOVES = ROOK_MOVES(FROM) & ~(white[ALL] | black[ALL]); MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO = asm_tzcnt(MOVES); \
        EVALUATE_MOVE(ROOK, FROM_BIT, ZOBRIST, TO, PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
for (uint64_t QUEENS = PIECE_STATE[QUEEN]; QUEENS != 0; QUEENS = asm_blsr(QUEENS)) { \
    uint64_t FROM = asm_tzcnt(QUEENS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    uint64_t ZOBRIST = zobrist ^ zobrist_GET(PIECE_STATE, FROM, QUEEN); \
    for ( \
        uint64_t MOVES = (BISHOP_MOVES(FROM) | ROOK_MOVES(FROM)) & ~(white[ALL] | black[ALL]); \
        MOVES != 0; \
        MOVES = asm_blsr(MOVES) \
    ) { \
        uint64_t TO = asm_tzcnt(MOVES); \
        EVALUATE_MOVE(QUEEN, FROM_BIT, ZOBRIST, TO, PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
{ \
    uint64_t FROM = asm_tzcnt(PIECE_STATE[KING]); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    uint64_t ZOBRIST = zobrist ^ zobrist_GET(PIECE_STATE, FROM, KING); \
    for (uint64_t MOVES = gen_kingMoves[FROM] & ~(white[ALL] | black[ALL]); MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO = asm_tzcnt(MOVES); \
        EVALUATE_MOVE(KING, FROM_BIT, ZOBRIST, TO, PIECE_STATE, MOVE_ACTION, __VA_ARGS__) \
    } \
} \
STOP_SEARCH: \
/* Update transposition table. */ \
TT_ENTRY |= tt_SHIFT_SCORE(BEST); \
TT_ENTRY |= tt_SHIFT_DEPTH(remainingDepth); \
tt[tt_INDEX(zobrist)] = TT_ENTRY; \
return BEST;

static int32_t evaluateWhiteMoves(uint64_t zobrist, int32_t beta, int32_t alpha, int32_t remainingDepth, int32_t score);
static int32_t evaluateBlackMoves(uint64_t zobrist, int32_t alpha, int32_t beta, int32_t remainingDepth, int32_t score) {
    EVALUATE_MOVES(
        int32_t SCORE = evaluateWhiteMoves(NEW_ZOBRIST, beta, alpha, remainingDepth, score);
        ,
        -, FILE_H, FILE_A, RANK2, >>, <<, black, white, false
        ,
        if (TT_SCORE <= alpha) return TT_SCORE;
        if (TT_SCORE < beta) {
            beta = TT_SCORE;
        }
        ,
        if (TT_SCORE >= beta) return TT_SCORE;
        if (TT_SCORE > alpha) {
            alpha = TT_SCORE;
        }
        ,
        if (SCORE < BEST) {
            BEST = SCORE;
            if (BEST <= alpha) {
                TT_ENTRY |= tt_SCORE_TYPE_CUTOFF;
                goto STOP_SEARCH;
            }
            if (BEST < beta) {
                TT_ENTRY |= tt_SCORE_TYPE_EXACT;
                beta = BEST;
            }
        }
    )
}

static int32_t evaluateWhiteMoves(uint64_t zobrist, int32_t beta, int32_t alpha, int32_t remainingDepth, int32_t score) {
    EVALUATE_MOVES(
        int32_t SCORE = evaluateBlackMoves(NEW_ZOBRIST, alpha, beta, remainingDepth - 1, score);
        ,
        +, FILE_A, FILE_H, RANK7, <<, >>, white, black, remainingDepth == 0
        ,
        if (TT_SCORE >= beta) return TT_SCORE;
        if (TT_SCORE > alpha) {
            alpha = TT_SCORE;
        }
        ,
        if (TT_SCORE <= alpha) return TT_SCORE;
        if (TT_SCORE < beta) {
            beta = TT_SCORE;
        }
        ,
        if (SCORE > BEST) {
            BEST = SCORE;
            if (BEST >= beta) {
                TT_ENTRY |= tt_SCORE_TYPE_CUTOFF;
                goto STOP_SEARCH;
            }
            if (BEST > alpha) {
                TT_ENTRY |= tt_SCORE_TYPE_EXACT;
                alpha = BEST;
            }
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

    int32_t score = 1000 * ((int32_t)asm_popcnt(white[KING]) - (int32_t)asm_popcnt(black[KING]));
    score += 9 * ((int32_t)asm_popcnt(white[QUEEN]) - (int32_t)asm_popcnt(black[QUEEN]));
    score += 5 * ((int32_t)asm_popcnt(white[ROOK]) - (int32_t)asm_popcnt(black[ROOK]));
    score += 3 * ((int32_t)asm_popcnt(white[BISHOP]) - (int32_t)asm_popcnt(black[BISHOP]));
    score += 3 * ((int32_t)asm_popcnt(white[KNIGHT]) - (int32_t)asm_popcnt(black[KNIGHT]));
    score += 1 * ((int32_t)asm_popcnt(white[PAWN]) - (int32_t)asm_popcnt(black[PAWN]));
    return score;
}

static void findMoves(uint8_t *board, int32_t initialScore, uint64_t zobrist) {
    for (uint64_t from = 0; from < 64; ++from) {
        uint8_t piece = board[from];
        if (!(piece & WHITE)) continue;

        piece &= PIECE_MASK;
        uint64_t fromBit = (uint64_t)1 << from;
        uint64_t zobristWithFrom = zobrist ^ zobrist_GET(white, from, piece);
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
            uint64_t capture = board[to] & PIECE_MASK;

            int32_t absoluteScore;
            if (piece == PAWN && (fromBit & RANK7)) {
                if (capture != NONE) {
                    EVALUATE_PAWN_CAPTURE_PROMOTE_MOVE(
                        capture, fromBit, zobristWithFrom, to, white, black,
                        absoluteScore = countScore();
                    )
                } else {
                    EVALUATE_PAWN_PROMOTE_MOVE(
                        fromBit, zobristWithFrom, to, white,
                        absoluteScore = countScore();
                    )
                }
            } else {
                if (capture != NONE) {
                    EVALUATE_CAPTURE_MOVE(
                        capture, fromBit, zobristWithFrom, to, piece, white, black,
                        absoluteScore = countScore();
                    )
                } else {
                    EVALUATE_MOVE(
                        piece, fromBit, zobristWithFrom, to, white,
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

static int32_t evaluateRootMoves(uint64_t zobrist, int32_t initialScore, uint8_t *board, int32_t alpha, int32_t beta, int32_t remainingDepth) {
    int32_t best = -INFINITY;
    for (int32_t i = 0; i < numFoundMoves; ++i) {
        struct move *move = &foundMoves[i];
        uint8_t piece = board[move->from] & PIECE_MASK;
        uint8_t capture = board[move->to] & PIECE_MASK;
        uint64_t fromBit = (uint64_t)1 << move->from;
        uint64_t zobristWithFrom = zobrist ^ zobrist_GET(white, move->from, piece);
        uint64_t toBit = (uint64_t)1 << move->to;

        if (toBit & black[KING]) {
            // move->score already correct from findMoves().
        } else if (piece == PAWN && (fromBit & RANK7)) {
            if (capture != NONE) {
                EVALUATE_PAWN_CAPTURE_PROMOTE_MOVE(
                    capture, fromBit, zobristWithFrom, move->to, white, black,
                    move->score = evaluateBlackMoves(NEW_ZOBRIST, alpha, beta, remainingDepth - 1, countScore() - initialScore);
                )
            } else {
                EVALUATE_PAWN_PROMOTE_MOVE(
                    fromBit, zobristWithFrom, move->to, white,
                    move->score = evaluateBlackMoves(NEW_ZOBRIST, alpha, beta, remainingDepth - 1, countScore() - initialScore);
                )
            }
        } else {
            if (capture != NONE) {
                EVALUATE_CAPTURE_MOVE(
                    capture, fromBit, zobristWithFrom, move->to, piece, white, black,
                    move->score = evaluateBlackMoves(NEW_ZOBRIST, alpha, beta, remainingDepth - 1, countScore() - initialScore);
                )
            } else {
                EVALUATE_MOVE(
                    piece, fromBit, zobristWithFrom, move->to, white,
                    move->score = evaluateBlackMoves(NEW_ZOBRIST, alpha, beta, remainingDepth - 1, countScore() - initialScore);
                )
            }
        }
        move->exactScore = (move->score > alpha) && (move->score < beta);

        if (move->score > best) {
            best = move->score;
            if (best >= beta) return best;
            if (best > alpha) alpha = best;
        }
    }
    return best;
}

static int32_t makeMove(bool isHost, uint8_t *board, hc_UNUSED int32_t lastMoveFrom, hc_UNUSED int32_t lastMoveTo, int32_t *moveFrom, int32_t *moveTo) {
    uint64_t zobrist;
    init(isHost, board, &zobrist);
    int32_t initialScore = countScore();
    findMoves(board, initialScore, zobrist);
    int32_t targetDepth = 5; // Number of moves for each side before we evaluate the score.

    for (int32_t remainingDepth = 1; remainingDepth <= targetDepth; ++remainingDepth) {
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
            int32_t score = evaluateRootMoves(zobrist, initialScore, board, alpha, beta, remainingDepth);
            debug_printNum(" = ", score, "\n");

            if (score <= alpha) {
                alpha = score - 1;
                beta = score + 1; // TODO: Keep when search becomes instable?
            } else if (score >= beta) {
                beta = score + 1;
                alpha = score - 1; // TODO: Keep when search becomes instable?
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

    if (tt_init() < 0) return 2;

    client_create(&client, makeMove);
    uint8_t address[] = { 127, 0, 0, 1 };
    int32_t status = client_run(&client, &address[0], 8089, roomId);
    debug_printNum("Status: ", status, "\n");
    return 0;
}