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

static int32_t evaluateWhiteMoves(int32_t score, int32_t alpha, int32_t beta, int32_t remainingDepth);

struct move {
    int32_t from;
    int32_t to;
};

struct pieceState {
    uint64_t all;
    uint64_t pawns;
    uint64_t knights;
    uint64_t bishops;
    uint64_t rooks;
    uint64_t queens;
    uint64_t king;
};

static struct pieceState white;
static struct pieceState black;

static void init(bool isHost, uint8_t *board) {
    memset(&white, 0, sizeof(white));
    memset(&black, 0, sizeof(black));

    for (int32_t i = 0; i < 64; ++i) {
        uint8_t piece = board[i];
        uint64_t bit = (uint64_t)1 << i;
        if (piece != protocol_NO_PIECE) {
            if (!isHost) piece ^= (protocol_WHITE_FLAG | protocol_BLACK_FLAG);

            if (piece & protocol_WHITE_FLAG) {
                white.all |= bit;
                switch (piece & protocol_PIECE_MASK) {
                    case protocol_PAWN: {
                        white.pawns |= bit;
                        break;
                    }
                    case protocol_KNIGHT: {
                        white.knights |= bit;
                        break;
                    }
                    case protocol_BISHOP: {
                        white.bishops |= bit;
                        break;
                    }
                    case protocol_ROOK: {
                        white.rooks |= bit;
                        break;
                    }
                    case protocol_QUEEN: {
                        white.queens |= bit;
                        break;
                    }
                    case protocol_KING: {
                        white.king |= bit;
                        break;
                    }
                    default: hc_UNREACHABLE;
                }
            } else {
                black.all |= bit;
                switch (piece & protocol_PIECE_MASK) {
                    case protocol_PAWN: {
                        black.pawns |= bit;
                        break;
                    }
                    case protocol_KNIGHT: {
                        black.knights |= bit;
                        break;
                    }
                    case protocol_BISHOP: {
                        black.bishops |= bit;
                        break;
                    }
                    case protocol_ROOK: {
                        black.rooks |= bit;
                        break;
                    }
                    case protocol_QUEEN: {
                        black.queens |= bit;
                        break;
                    }
                    case protocol_KING: {
                        black.king |= bit;
                        break;
                    }
                    default: hc_UNREACHABLE;
                }
            }
        }
    }
}

#define EVALUATE_MOVE( \
SOURCE, \
PIECE_STATE, \
TO_BIT, \
FROM_BIT, \
OPP_EVALUATE_FN, \
SCORE, \
ALPHABETA_BETTER_OP, \
ALPHABETA_BETTER_EQUAL_OP, \
ALPHABETA, \
OPP_ALPHABETA \
) \
PIECE_STATE.all ^= (FROM_BIT | TO_BIT); \
SOURCE ^= (FROM_BIT | TO_BIT); \
int32_t SCORE = OPP_EVALUATE_FN(score, ALPHABETA, OPP_ALPHABETA, remainingDepth - 1); \
SOURCE ^= (FROM_BIT | TO_BIT); \
PIECE_STATE.all ^= (FROM_BIT | TO_BIT); \
if (SCORE ALPHABETA_BETTER_EQUAL_OP OPP_ALPHABETA) return OPP_ALPHABETA; \
if (SCORE ALPHABETA_BETTER_OP ALPHABETA) ALPHABETA = SCORE;

#define EVALUATE_CAPTURE_MOVE( \
TARGET, \
SOURCE, \
PIECE_STATE, \
OPP_PIECE_STATE, \
TO_BIT, \
FROM_BIT, \
OPP_EVALUATE_FN, \
SCORE, \
ALPHABETA_BETTER_OP, \
ALPHABETA_BETTER_EQUAL_OP, \
ALPHABETA, \
OPP_ALPHABETA \
) \
OPP_PIECE_STATE.all ^= TO_BIT; \
TARGET ^= TO_BIT; \
PIECE_STATE.all ^= (FROM_BIT | TO_BIT); /* TODO: does this optimize well? */ \
SOURCE ^= (FROM_BIT | TO_BIT); \
int32_t SCORE = OPP_EVALUATE_FN(score, ALPHABETA, OPP_ALPHABETA, remainingDepth - 1); \
SOURCE ^= (FROM_BIT | TO_BIT); \
PIECE_STATE.all ^= (FROM_BIT | TO_BIT); \
TARGET ^= TO_BIT; \
OPP_PIECE_STATE.all ^= TO_BIT; \
if (SCORE ALPHABETA_BETTER_EQUAL_OP OPP_ALPHABETA) return OPP_ALPHABETA; \
if (SCORE ALPHABETA_BETTER_OP ALPHABETA) ALPHABETA = SCORE;

#define PAWN_CAPTURE_PROMOTE_MOVES( \
TARGET, \
PROMOTE_MASK, \
OPP_EVALUATE_FN, \
LEFT_FILE, \
RIGHT_FILE, \
SHIFT_UP_OP, \
SHIFT_DOWN_OP, \
ALPHABETA_BETTER_OP, \
ALPHABETA_BETTER_EQUAL_OP, \
ALPHABETA, \
OPP_ALPHABETA, \
PIECE_STATE, \
OPP_PIECE_STATE \
) \
for (uint64_t PAWNS_LEFT = PIECE_STATE.pawns & PROMOTE_MASK & ~LEFT_FILE & (TARGET SHIFT_DOWN_OP 7); PAWNS_LEFT != 0; PAWNS_LEFT = asm_blsr(PAWNS_LEFT)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_LEFT); \
    uint64_t TO_BIT = (FROM_BIT SHIFT_UP_OP 7); \
    OPP_PIECE_STATE.all ^= TO_BIT; \
    TARGET ^= TO_BIT; \
    PIECE_STATE.queens ^= TO_BIT; \
    PIECE_STATE.pawns ^= FROM_BIT; \
    PIECE_STATE.all ^= (FROM_BIT | TO_BIT); \
    int32_t SCORE = OPP_EVALUATE_FN(score, ALPHABETA, OPP_ALPHABETA, remainingDepth - 1); \
    PIECE_STATE.all ^= (FROM_BIT | TO_BIT); \
    PIECE_STATE.pawns ^= FROM_BIT; \
    PIECE_STATE.queens ^= TO_BIT; \
    TARGET ^= TO_BIT; \
    OPP_PIECE_STATE.all ^= TO_BIT; \
    if (SCORE ALPHABETA_BETTER_EQUAL_OP OPP_ALPHABETA) return OPP_ALPHABETA; \
    if (SCORE ALPHABETA_BETTER_OP ALPHABETA) ALPHABETA = SCORE; \
} \
for (uint64_t PAWNS_RIGHT = PIECE_STATE.pawns & PROMOTE_MASK & ~RIGHT_FILE & (TARGET SHIFT_DOWN_OP 9); PAWNS_RIGHT != 0; PAWNS_RIGHT = asm_blsr(PAWNS_RIGHT)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_RIGHT); \
    uint64_t TO_BIT = (FROM_BIT SHIFT_UP_OP 9); \
    OPP_PIECE_STATE.all ^= TO_BIT; \
    TARGET ^= TO_BIT; \
    PIECE_STATE.queens ^= TO_BIT; \
    PIECE_STATE.pawns ^= FROM_BIT; \
    PIECE_STATE.all ^= (FROM_BIT | TO_BIT); \
    int32_t SCORE = OPP_EVALUATE_FN(score, ALPHABETA, OPP_ALPHABETA, remainingDepth - 1); \
    PIECE_STATE.all ^= (FROM_BIT | TO_BIT); \
    PIECE_STATE.pawns ^= FROM_BIT; \
    PIECE_STATE.queens ^= TO_BIT; \
    TARGET ^= TO_BIT; \
    OPP_PIECE_STATE.all ^= TO_BIT; \
    if (SCORE ALPHABETA_BETTER_EQUAL_OP OPP_ALPHABETA) return OPP_ALPHABETA; \
    if (SCORE ALPHABETA_BETTER_OP ALPHABETA) ALPHABETA = SCORE; \
}

#define CAPTURE_MOVES( \
TARGET, \
OPP_EVALUATE_FN, \
LEFT_FILE, \
RIGHT_FILE, \
PROMOTE_MASK, \
SHIFT_UP_OP, \
SHIFT_DOWN_OP, \
ALPHABETA_BETTER_OP, \
ALPHABETA_BETTER_EQUAL_OP, \
ALPHABETA, \
OPP_ALPHABETA, \
PIECE_STATE, \
OPP_PIECE_STATE \
) \
for (uint64_t PAWNS_LEFT = PIECE_STATE.pawns & ~PROMOTE_MASK & ~LEFT_FILE & (TARGET SHIFT_DOWN_OP 7); PAWNS_LEFT != 0; PAWNS_LEFT = asm_blsr(PAWNS_LEFT)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_LEFT); \
    uint64_t TO_BIT = (FROM_BIT SHIFT_UP_OP 7); \
    EVALUATE_CAPTURE_MOVE(TARGET, PIECE_STATE.pawns, PIECE_STATE, OPP_PIECE_STATE, TO_BIT, FROM_BIT, OPP_EVALUATE_FN, SCORE, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA) \
} \
for (uint64_t PAWNS_RIGHT = PIECE_STATE.pawns & ~PROMOTE_MASK & ~RIGHT_FILE & (TARGET SHIFT_DOWN_OP 9); PAWNS_RIGHT != 0; PAWNS_RIGHT = asm_blsr(PAWNS_RIGHT)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_RIGHT); \
    uint64_t TO_BIT = (FROM_BIT SHIFT_UP_OP 9); \
    EVALUATE_CAPTURE_MOVE(TARGET, PIECE_STATE.pawns, PIECE_STATE, OPP_PIECE_STATE, TO_BIT, FROM_BIT, OPP_EVALUATE_FN, SCORE, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA) \
} \
for (uint64_t KNIGHTS = PIECE_STATE.knights; KNIGHTS != 0; KNIGHTS = asm_blsr(KNIGHTS)) { \
    uint64_t FROM = asm_tzcnt(KNIGHTS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_knightMoves[FROM] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_CAPTURE_MOVE(TARGET, PIECE_STATE.knights, PIECE_STATE, OPP_PIECE_STATE, TO_BIT, FROM_BIT, OPP_EVALUATE_FN, SCORE, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA) \
    } \
} \
for (uint64_t BISHOPS = PIECE_STATE.bishops; BISHOPS != 0; BISHOPS = asm_blsr(BISHOPS)) { \
    uint64_t FROM = asm_tzcnt(BISHOPS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_bishopMoves[(FROM << 9) | asm_pext(PIECE_STATE.all | OPP_PIECE_STATE.all, gen_bishopMasks[FROM])] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_CAPTURE_MOVE(TARGET, PIECE_STATE.bishops, PIECE_STATE, OPP_PIECE_STATE, TO_BIT, FROM_BIT, OPP_EVALUATE_FN, SCORE, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA) \
    } \
} \
for (uint64_t ROOKS = PIECE_STATE.rooks; ROOKS != 0; ROOKS = asm_blsr(ROOKS)) { \
    uint64_t FROM = asm_tzcnt(ROOKS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_rookMoves[(FROM << 12) | asm_pext(PIECE_STATE.all | OPP_PIECE_STATE.all, gen_rookMasks[FROM])] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_CAPTURE_MOVE(TARGET, PIECE_STATE.rooks, PIECE_STATE, OPP_PIECE_STATE, TO_BIT, FROM_BIT, OPP_EVALUATE_FN, SCORE, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA) \
    } \
} \
for (uint64_t QUEENS = PIECE_STATE.queens; QUEENS != 0; QUEENS = asm_blsr(QUEENS)) { \
    uint64_t FROM = asm_tzcnt(QUEENS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for ( \
        uint64_t MOVES = (gen_bishopMoves[(FROM << 9) | asm_pext(PIECE_STATE.all | OPP_PIECE_STATE.all, gen_bishopMasks[FROM])] | gen_rookMoves[(FROM << 12) | asm_pext(PIECE_STATE.all | OPP_PIECE_STATE.all, gen_rookMasks[FROM])]) & TARGET; \
        MOVES != 0; \
        MOVES = asm_blsr(MOVES) \
    ) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_CAPTURE_MOVE(TARGET, PIECE_STATE.queens, PIECE_STATE, OPP_PIECE_STATE, TO_BIT, FROM_BIT, OPP_EVALUATE_FN, SCORE, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA) \
    } \
} \
{ \
    uint64_t FROM = asm_tzcnt(PIECE_STATE.king); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_kingMoves[FROM] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_CAPTURE_MOVE(TARGET, PIECE_STATE.king, PIECE_STATE, OPP_PIECE_STATE, TO_BIT, FROM_BIT, OPP_EVALUATE_FN, SCORE, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA) \
    } \
}

#define EVALUATE_MOVES( \
    OPP_EVALUATE_FN, \
    SCORE_OP, \
    LEFT_FILE, \
    RIGHT_FILE, \
    PROMOTE_MASK, \
    SHIFT_UP_OP, \
    SHIFT_DOWN_OP, \
    ALPHABETA_BETTER_OP, \
    ALPHABETA_BETTER_EQUAL_OP, \
    ALPHABETA, \
    OPP_ALPHABETA, \
    PIECE_STATE, \
    OPP_PIECE_STATE \
) \
/* Generate all tiles that we attack (except for pawn promotions). */ \
uint64_t ATTACKED = ( \
    ((PIECE_STATE.pawns & ~PROMOTE_MASK & ~LEFT_FILE) SHIFT_UP_OP 7) | /* Left */ \
    ((PIECE_STATE.pawns & ~PROMOTE_MASK & ~RIGHT_FILE) SHIFT_UP_OP 9)  /* Right */ \
); \
ATTACKED |= gen_kingMoves[asm_tzcnt(PIECE_STATE.king)]; \
for (uint64_t KNIGHTS = PIECE_STATE.knights; KNIGHTS != 0; KNIGHTS = asm_blsr(KNIGHTS)) { \
    ATTACKED |= gen_knightMoves[asm_tzcnt(KNIGHTS)]; \
} \
for (uint64_t BISHOPS_AND_QUEENS = PIECE_STATE.bishops | PIECE_STATE.queens; BISHOPS_AND_QUEENS != 0; BISHOPS_AND_QUEENS = asm_blsr(BISHOPS_AND_QUEENS)) { \
    uint64_t FROM = asm_tzcnt(BISHOPS_AND_QUEENS); \
    ATTACKED |= gen_bishopMoves[(FROM << 9) | asm_pext(PIECE_STATE.all | OPP_PIECE_STATE.all, gen_bishopMasks[FROM])]; \
} \
for (uint64_t ROOKS_AND_QUEENS = PIECE_STATE.rooks | PIECE_STATE.queens; ROOKS_AND_QUEENS != 0; ROOKS_AND_QUEENS = asm_blsr(ROOKS_AND_QUEENS)) { \
    uint64_t FROM = asm_tzcnt(ROOKS_AND_QUEENS); \
    ATTACKED |= gen_rookMoves[(FROM << 12) | asm_pext(PIECE_STATE.all | OPP_PIECE_STATE.all, gen_rookMasks[FROM])]; \
} \
 \
if (ATTACKED & OPP_PIECE_STATE.king) return score SCORE_OP (10000 + remainingDepth); \
 \
if (PIECE_STATE.pawns & PROMOTE_MASK) { \
    /* We have potential pawn promotions. */ \
    uint64_t PROMOTE_ATTACKED = ( \
        ((PIECE_STATE.pawns & PROMOTE_MASK & ~LEFT_FILE) SHIFT_UP_OP 7) | /* Left */ \
        ((PIECE_STATE.pawns & PROMOTE_MASK & ~RIGHT_FILE) SHIFT_UP_OP 9)  /* Right */ \
    ); \
    if (PROMOTE_ATTACKED & OPP_PIECE_STATE.king) return score SCORE_OP (10000 + remainingDepth); \
    if (PROMOTE_ATTACKED & OPP_PIECE_STATE.queens) { \
        score += SCORE_OP (9 + 8); \
        if (remainingDepth == 0) return score; \
        PAWN_CAPTURE_PROMOTE_MOVES(OPP_PIECE_STATE.queens, PROMOTE_MASK, OPP_EVALUATE_FN, LEFT_FILE, RIGHT_FILE, SHIFT_UP_OP, SHIFT_DOWN_OP, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA, PIECE_STATE, OPP_PIECE_STATE) \
        score -= SCORE_OP (9 + 8); \
    } \
    if (PROMOTE_ATTACKED & OPP_PIECE_STATE.rooks) { \
        score += SCORE_OP (5 + 8); \
        if (remainingDepth == 0) return score; \
        PAWN_CAPTURE_PROMOTE_MOVES(OPP_PIECE_STATE.rooks, PROMOTE_MASK, OPP_EVALUATE_FN, LEFT_FILE, RIGHT_FILE, SHIFT_UP_OP, SHIFT_DOWN_OP, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA, PIECE_STATE, OPP_PIECE_STATE) \
        score -= SCORE_OP (5 + 8); \
    } \
    if (PROMOTE_ATTACKED & OPP_PIECE_STATE.bishops) { \
        score += SCORE_OP (3 + 8); \
        if (remainingDepth == 0) return score; \
        PAWN_CAPTURE_PROMOTE_MOVES(OPP_PIECE_STATE.bishops, PROMOTE_MASK, OPP_EVALUATE_FN, LEFT_FILE, RIGHT_FILE, SHIFT_UP_OP, SHIFT_DOWN_OP, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA, PIECE_STATE, OPP_PIECE_STATE) \
        score -= SCORE_OP (3 + 8); \
    } \
    if (PROMOTE_ATTACKED & OPP_PIECE_STATE.knights) { \
        score += SCORE_OP (3 + 8); \
        if (remainingDepth == 0) return score; \
        PAWN_CAPTURE_PROMOTE_MOVES(OPP_PIECE_STATE.knights, PROMOTE_MASK, OPP_EVALUATE_FN, LEFT_FILE, RIGHT_FILE, SHIFT_UP_OP, SHIFT_DOWN_OP, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA, PIECE_STATE, OPP_PIECE_STATE) \
        score -= SCORE_OP (3 + 8); \
    } \
    if (PROMOTE_ATTACKED & OPP_PIECE_STATE.pawns) { \
        score += SCORE_OP (1 + 8); \
        if (remainingDepth == 0) return score; \
        PAWN_CAPTURE_PROMOTE_MOVES(OPP_PIECE_STATE.pawns, PROMOTE_MASK, OPP_EVALUATE_FN, LEFT_FILE, RIGHT_FILE, SHIFT_UP_OP, SHIFT_DOWN_OP, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA, PIECE_STATE, OPP_PIECE_STATE) \
        score -= SCORE_OP (1 + 8); \
    } \
    { \
        uint64_t PAWN_PROMOTIONS = PIECE_STATE.pawns & PROMOTE_MASK & (~(PIECE_STATE.all | OPP_PIECE_STATE.all) SHIFT_DOWN_OP 8); \
        if (PAWN_PROMOTIONS) { \
            score += SCORE_OP 8; \
            if (remainingDepth == 0) return score SCORE_OP !!(ATTACKED & OPP_PIECE_STATE.queens); /* 9 if we can capture a queen. */ \
            do { \
                uint64_t FROM_BIT = asm_blsi(PAWN_PROMOTIONS); \
                uint64_t TO_BIT = FROM_BIT SHIFT_UP_OP 8; \
                PIECE_STATE.queens ^= TO_BIT; \
                PIECE_STATE.all ^= FROM_BIT; \
                PIECE_STATE.pawns ^= (FROM_BIT | TO_BIT); \
                int32_t SCORE = OPP_EVALUATE_FN(score, ALPHABETA, OPP_ALPHABETA, remainingDepth - 1); \
                PIECE_STATE.pawns ^= (FROM_BIT | TO_BIT); \
                PIECE_STATE.all ^= FROM_BIT; \
                PIECE_STATE.queens ^= TO_BIT; \
                if (SCORE ALPHABETA_BETTER_EQUAL_OP OPP_ALPHABETA) return OPP_ALPHABETA; \
                if (SCORE ALPHABETA_BETTER_OP ALPHABETA) ALPHABETA = SCORE; \
                PAWN_PROMOTIONS = asm_blsr(PAWN_PROMOTIONS); \
            } while (PAWN_PROMOTIONS); \
            score -= SCORE_OP 8; \
        } \
    } \
} \
if (ATTACKED & OPP_PIECE_STATE.queens) { \
    score += SCORE_OP 9; \
    if (remainingDepth == 0) return score; \
    CAPTURE_MOVES(OPP_PIECE_STATE.queens, OPP_EVALUATE_FN, LEFT_FILE, RIGHT_FILE, PROMOTE_MASK, SHIFT_UP_OP, SHIFT_DOWN_OP, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA, PIECE_STATE, OPP_PIECE_STATE) \
    score -= SCORE_OP 9; \
} \
if (ATTACKED & OPP_PIECE_STATE.rooks) { \
    score += SCORE_OP 5; \
    if (remainingDepth == 0) return score; \
    CAPTURE_MOVES(OPP_PIECE_STATE.rooks, OPP_EVALUATE_FN, LEFT_FILE, RIGHT_FILE, PROMOTE_MASK, SHIFT_UP_OP, SHIFT_DOWN_OP, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA, PIECE_STATE, OPP_PIECE_STATE) \
    score -= SCORE_OP 5; \
} \
if (ATTACKED & OPP_PIECE_STATE.bishops) { \
    score += SCORE_OP 3; \
    if (remainingDepth == 0) return score; \
    CAPTURE_MOVES(OPP_PIECE_STATE.bishops, OPP_EVALUATE_FN, LEFT_FILE, RIGHT_FILE, PROMOTE_MASK, SHIFT_UP_OP, SHIFT_DOWN_OP, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA, PIECE_STATE, OPP_PIECE_STATE) \
    score -= SCORE_OP 3; \
} \
if (ATTACKED & OPP_PIECE_STATE.knights) { \
    score += SCORE_OP 3; \
    if (remainingDepth == 0) return score; \
    CAPTURE_MOVES(OPP_PIECE_STATE.knights, OPP_EVALUATE_FN, LEFT_FILE, RIGHT_FILE, PROMOTE_MASK, SHIFT_UP_OP, SHIFT_DOWN_OP, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA, PIECE_STATE, OPP_PIECE_STATE) \
    score -= SCORE_OP 3; \
} \
if (ATTACKED & OPP_PIECE_STATE.pawns) { \
    score += SCORE_OP 1; \
    if (remainingDepth == 0) return score; \
    CAPTURE_MOVES(OPP_PIECE_STATE.pawns, OPP_EVALUATE_FN, LEFT_FILE, RIGHT_FILE, PROMOTE_MASK, SHIFT_UP_OP, SHIFT_DOWN_OP, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA, PIECE_STATE, OPP_PIECE_STATE) \
    score -= SCORE_OP 1; \
} \
if (remainingDepth == 0) return score; \
/* TODO: What move order for non captures? */ \
for (uint64_t KNIGHTS = PIECE_STATE.knights; KNIGHTS != 0; KNIGHTS = asm_blsr(KNIGHTS)) { \
    uint64_t FROM = asm_tzcnt(KNIGHTS); \
    uint64_t MOVES = gen_knightMoves[FROM] & ~(PIECE_STATE.all | OPP_PIECE_STATE.all); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        PIECE_STATE.all ^= (FROM_BIT | TO_BIT); \
        PIECE_STATE.knights ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = OPP_EVALUATE_FN(score, ALPHABETA, OPP_ALPHABETA, remainingDepth - 1); \
        PIECE_STATE.knights ^= (FROM_BIT | TO_BIT); \
        PIECE_STATE.all ^= (FROM_BIT | TO_BIT); \
        if (SCORE ALPHABETA_BETTER_EQUAL_OP OPP_ALPHABETA) return OPP_ALPHABETA; \
        if (SCORE ALPHABETA_BETTER_OP ALPHABETA) ALPHABETA = SCORE; \
    } \
} \
for (uint64_t PAWNS_FORWARD = PIECE_STATE.pawns & ~PROMOTE_MASK & (~(PIECE_STATE.all | OPP_PIECE_STATE.all) SHIFT_DOWN_OP 8); PAWNS_FORWARD != 0; PAWNS_FORWARD = asm_blsr(PAWNS_FORWARD)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_FORWARD); \
    uint64_t TO_BIT = FROM_BIT SHIFT_UP_OP 8; \
    EVALUATE_MOVE(PIECE_STATE.pawns, PIECE_STATE, TO_BIT, FROM_BIT, OPP_EVALUATE_FN, SCORE, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA) \
} \
for (uint64_t BISHOPS = PIECE_STATE.bishops; BISHOPS != 0; BISHOPS = asm_blsr(BISHOPS)) { \
    uint64_t FROM = asm_tzcnt(BISHOPS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_bishopMoves[(FROM << 9) | asm_pext(PIECE_STATE.all | OPP_PIECE_STATE.all, gen_bishopMasks[FROM])] & ~(PIECE_STATE.all | OPP_PIECE_STATE.all); MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_MOVE(PIECE_STATE.bishops, PIECE_STATE, TO_BIT, FROM_BIT, OPP_EVALUATE_FN, SCORE, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA) \
    } \
} \
for (uint64_t ROOKS = PIECE_STATE.rooks; ROOKS != 0; ROOKS = asm_blsr(ROOKS)) { \
    uint64_t FROM = asm_tzcnt(ROOKS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_rookMoves[(FROM << 12) | asm_pext(PIECE_STATE.all | OPP_PIECE_STATE.all, gen_rookMasks[FROM])] & ~(PIECE_STATE.all | OPP_PIECE_STATE.all); MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_MOVE(PIECE_STATE.rooks, PIECE_STATE, TO_BIT, FROM_BIT, OPP_EVALUATE_FN, SCORE, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA) \
    } \
} \
for (uint64_t QUEENS = PIECE_STATE.queens; QUEENS != 0; QUEENS = asm_blsr(QUEENS)) { \
    uint64_t FROM = asm_tzcnt(QUEENS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for ( \
        uint64_t MOVES = (gen_bishopMoves[(FROM << 9) | asm_pext(PIECE_STATE.all | OPP_PIECE_STATE.all, gen_bishopMasks[FROM])] | gen_rookMoves[(FROM << 12) | asm_pext(PIECE_STATE.all | OPP_PIECE_STATE.all, gen_rookMasks[FROM])]) & ~(PIECE_STATE.all | OPP_PIECE_STATE.all); \
        MOVES != 0; \
        MOVES = asm_blsr(MOVES) \
    ) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_MOVE(PIECE_STATE.queens, PIECE_STATE, TO_BIT, FROM_BIT, OPP_EVALUATE_FN, SCORE, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA) \
    } \
} \
{ \
    uint64_t FROM = asm_tzcnt(PIECE_STATE.king); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_kingMoves[FROM] & ~(PIECE_STATE.all | OPP_PIECE_STATE.all); MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        EVALUATE_MOVE(PIECE_STATE.king, PIECE_STATE, TO_BIT, FROM_BIT, OPP_EVALUATE_FN, SCORE, ALPHABETA_BETTER_OP, ALPHABETA_BETTER_EQUAL_OP, ALPHABETA, OPP_ALPHABETA) \
    } \
} \
return ALPHABETA;

static int32_t evaluateBlackMoves(int32_t score, int32_t alpha, int32_t beta, int32_t remainingDepth) {
EVALUATE_MOVES(evaluateWhiteMoves, -, FILE_H, FILE_A, RANK2, >>, <<, <, <=, beta, alpha, black, white)
}

static int32_t evaluateWhiteMoves(int32_t score, int32_t beta, int32_t alpha, int32_t remainingDepth) {
EVALUATE_MOVES(evaluateBlackMoves, +, FILE_A, FILE_H, RANK7, <<, >>, >, >=, alpha, beta, white, black)
}

#define WHITE_PAWN_CAPTURE_MOVES_INITIAL(TARGET, MASK, IS_KING) \
for (uint64_t PAWNS_LEFT = white.pawns & MASK & ~FILE_A & (TARGET >> 7); PAWNS_LEFT != 0; PAWNS_LEFT = asm_blsr(PAWNS_LEFT)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_LEFT); \
    uint64_t TO_BIT = (FROM_BIT << 7); \
    black.all ^= TO_BIT; \
    TARGET ^= TO_BIT; \
    white.all ^= (FROM_BIT | TO_BIT); \
    white.pawns ^= (FROM_BIT | TO_BIT); \
    int32_t SCORE = score; \
    if (!IS_KING) SCORE = evaluateBlackMoves(score, alpha, beta, remainingDepth - 1); \
    if (SCORE > alpha) { \
        alpha = SCORE; \
        bestMove = (struct move) { \
            .from = (int32_t)asm_tzcnt(FROM_BIT), \
            .to = (int32_t)asm_tzcnt(TO_BIT), \
        }; \
    } \
    white.pawns ^= (FROM_BIT | TO_BIT); \
    white.all ^= (FROM_BIT | TO_BIT); \
    TARGET ^= TO_BIT; \
    black.all ^= TO_BIT; \
} \
for (uint64_t PAWNS_RIGHT = white.pawns & ~FILE_H & (TARGET >> 9); PAWNS_RIGHT != 0; PAWNS_RIGHT = asm_blsr(PAWNS_RIGHT)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_RIGHT); \
    uint64_t TO_BIT = (FROM_BIT << 9); \
    black.all ^= TO_BIT; \
    TARGET ^= TO_BIT; \
    white.all ^= (FROM_BIT | TO_BIT); \
    white.pawns ^= (FROM_BIT | TO_BIT); \
    int32_t SCORE = score; \
    if (!IS_KING) SCORE = evaluateBlackMoves(score, alpha, beta, remainingDepth - 1); \
    if (SCORE > alpha) { \
        alpha = SCORE; \
        bestMove = (struct move) { \
            .from = (int32_t)asm_tzcnt(FROM_BIT), \
            .to = (int32_t)asm_tzcnt(TO_BIT), \
        }; \
    } \
    white.pawns ^= (FROM_BIT | TO_BIT); \
    white.all ^= (FROM_BIT | TO_BIT); \
    TARGET ^= TO_BIT; \
    black.all ^= TO_BIT; \
}

#define WHITE_CAPTURE_MOVES_INITIAL(TARGET, IS_KING) \
WHITE_PAWN_CAPTURE_MOVES_INITIAL(TARGET, ~RANK7, IS_KING) \
uint64_t KNIGHTS = white.knights; \
for (; KNIGHTS != 0; KNIGHTS = asm_blsr(KNIGHTS)) { \
    uint64_t FROM = asm_tzcnt(KNIGHTS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_knightMoves[FROM] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        black.all ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        white.all ^= (FROM_BIT | TO_BIT); \
        white.knights ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = score; \
        if (!IS_KING) SCORE = evaluateBlackMoves(score, alpha, beta, remainingDepth - 1); \
        if (SCORE > alpha) { \
            alpha = SCORE; \
            bestMove = (struct move) { \
                .from = (int32_t)FROM, \
                .to = (int32_t)asm_tzcnt(MOVES), \
            }; \
        } \
        white.knights ^= (FROM_BIT | TO_BIT); \
        white.all ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        black.all ^= TO_BIT; \
    } \
} \
for (uint64_t BISHOPS = white.bishops; BISHOPS != 0; BISHOPS = asm_blsr(BISHOPS)) { \
    uint64_t FROM = asm_tzcnt(BISHOPS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_bishopMoves[(FROM << 9) | asm_pext(white.all | black.all, gen_bishopMasks[FROM])] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        black.all ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        white.all ^= (FROM_BIT | TO_BIT); \
        white.bishops ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = score; \
        if (!IS_KING) SCORE = evaluateBlackMoves(score, alpha, beta, remainingDepth - 1); \
        if (SCORE > alpha) { \
            alpha = SCORE; \
            bestMove = (struct move) { \
                .from = (int32_t)FROM, \
                .to = (int32_t)asm_tzcnt(MOVES), \
            }; \
        } \
        white.bishops ^= (FROM_BIT | TO_BIT); \
        white.all ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        black.all ^= TO_BIT; \
    } \
} \
for (uint64_t ROOKS = white.rooks; ROOKS != 0; ROOKS = asm_blsr(ROOKS)) { \
    uint64_t FROM = asm_tzcnt(ROOKS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_rookMoves[(FROM << 12) | asm_pext(white.all | black.all, gen_rookMasks[FROM])] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        black.all ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        white.all ^= (FROM_BIT | TO_BIT); \
        white.rooks ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = score; \
        if (!IS_KING) SCORE = evaluateBlackMoves(score, alpha, beta, remainingDepth - 1); \
        if (SCORE > alpha) { \
            alpha = SCORE; \
            bestMove = (struct move) { \
                .from = (int32_t)FROM, \
                .to = (int32_t)asm_tzcnt(MOVES), \
            }; \
        } \
        white.rooks ^= (FROM_BIT | TO_BIT); \
        white.all ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        black.all ^= TO_BIT; \
    } \
} \
for (uint64_t QUEENS = white.queens; QUEENS != 0; QUEENS = asm_blsr(QUEENS)) { \
    uint64_t FROM = asm_tzcnt(QUEENS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for ( \
        uint64_t MOVES = (gen_bishopMoves[(FROM << 9) | asm_pext(white.all | black.all, gen_bishopMasks[FROM])] | gen_rookMoves[(FROM << 12) | asm_pext(white.all | black.all, gen_rookMasks[FROM])]) & TARGET; \
        MOVES != 0; \
        MOVES = asm_blsr(MOVES) \
    ) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        black.all ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        white.all ^= (FROM_BIT | TO_BIT); \
        white.queens ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = score; \
        if (!IS_KING) SCORE = evaluateBlackMoves(score, alpha, beta, remainingDepth - 1); \
        if (SCORE > alpha) { \
            alpha = SCORE; \
            bestMove = (struct move) { \
                .from = (int32_t)FROM, \
                .to = (int32_t)asm_tzcnt(MOVES), \
            }; \
        } \
        white.queens ^= (FROM_BIT | TO_BIT); \
        white.all ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        black.all ^= TO_BIT; \
    } \
} \
{ \
    uint64_t FROM = asm_tzcnt(white.king); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_kingMoves[FROM] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        black.all ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        white.all ^= (FROM_BIT | TO_BIT); \
        white.king ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = score; \
        if (!IS_KING) SCORE = evaluateBlackMoves(score, alpha, beta, remainingDepth - 1); \
        if (SCORE > alpha) { \
            alpha = SCORE; \
            bestMove = (struct move) { \
                .from = (int32_t)FROM, \
                .to = (int32_t)asm_tzcnt(MOVES), \
            }; \
        } \
        white.king ^= (FROM_BIT | TO_BIT); \
        white.all ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        black.all ^= TO_BIT; \
    } \
}

static int32_t makeMove(bool isHost, uint8_t *board, hc_UNUSED int32_t lastMoveFrom, hc_UNUSED int32_t lastMoveTo, int32_t *moveFrom, int32_t *moveTo) {
    init(isHost, board);
    int32_t remainingDepth = 8;
    int32_t score = 0;
    int32_t alpha = INT32_MIN;
    int32_t beta = INT32_MAX;
    struct move bestMove = { .from = -1 };

    // Generate all tiles that we attack (except for pawn promotions).
    uint64_t attacked = (
        ((white.pawns & ~RANK7 & ~FILE_A) << 7) | // Left
        ((white.pawns & ~RANK7 & ~FILE_H) << 9)   // Right
    );
    attacked |= gen_kingMoves[asm_tzcnt(white.king)];

    for (uint64_t knights = white.knights; knights != 0; knights = asm_blsr(knights)) {
        attacked |= gen_knightMoves[asm_tzcnt(knights)];
    }

    for (uint64_t bishopsAndQueens = white.bishops | white.queens; bishopsAndQueens != 0; bishopsAndQueens = asm_blsr(bishopsAndQueens)) {
        uint64_t from = asm_tzcnt(bishopsAndQueens);
        attacked |= gen_bishopMoves[(from << 9) | asm_pext(white.all | black.all, gen_bishopMasks[from])];
    }

    for (uint64_t rooksAndQueens = white.rooks | white.queens; rooksAndQueens != 0; rooksAndQueens = asm_blsr(rooksAndQueens)) {
        uint64_t from = asm_tzcnt(rooksAndQueens);
        attacked |= gen_rookMoves[(from << 12) | asm_pext(white.all | black.all, gen_rookMasks[from])];
    }

    if (attacked & black.king) {
        score += (10000 + remainingDepth);
        WHITE_CAPTURE_MOVES_INITIAL(black.king, true)
        score -= (10000 + remainingDepth);
    }

    if (white.pawns & RANK7) {
        // We have potential pawn promotions.
        uint64_t promoteAttacked = (
            ((white.pawns & RANK7 & ~FILE_A) << 7) | // Left
            ((white.pawns & RANK7 & ~FILE_H) << 9)   // Right
        );
        if (promoteAttacked & black.king) {
            score += (10000 + remainingDepth);
            WHITE_PAWN_CAPTURE_MOVES_INITIAL(black.king, RANK7, true)
            score -= (10000 + remainingDepth);
        }

        if (promoteAttacked & black.queens) {
            score += (9 + 8);
            WHITE_PAWN_CAPTURE_MOVES_INITIAL(black.queens, RANK7, false)
            score -= (9 + 8);
        }

        if (promoteAttacked & black.rooks) {
            score += (5 + 8);
            WHITE_PAWN_CAPTURE_MOVES_INITIAL(black.rooks, RANK7, false)
            score -= (5 + 8);
        }

        if (promoteAttacked & black.bishops) {
            score += (3 + 8);
            WHITE_PAWN_CAPTURE_MOVES_INITIAL(black.bishops, RANK7, false)
            score -= (3 + 8);
        }

        if (promoteAttacked & black.knights) {
            score += (3 + 8);
            WHITE_PAWN_CAPTURE_MOVES_INITIAL(black.knights, RANK7, false)
            score -= (3 + 8);
        }

        if (promoteAttacked & black.pawns) {
            score += (1 + 8);
            WHITE_PAWN_CAPTURE_MOVES_INITIAL(black.pawns, RANK7, false)
            score -= (1 + 8);
        }

        {
            uint64_t pawnPromotions = white.pawns & RANK7 & (~(white.all | black.all) >> 8);
            if (pawnPromotions) {
                score += 8;
                do {
                    uint64_t fromBit = asm_blsi(pawnPromotions);
                    uint64_t toBit = fromBit << 8;
                    white.all ^= fromBit;
                    white.queens ^= toBit;
                    white.pawns ^= (fromBit | toBit);
                    int32_t newScore = evaluateBlackMoves(score, alpha, beta, remainingDepth - 1);
                    white.pawns ^= (fromBit | toBit);
                    white.queens ^= toBit;
                    white.all ^= fromBit;
                    if (newScore > alpha) {
                        alpha = newScore;
                        bestMove = (struct move) {
                            .from = (int32_t)asm_tzcnt(fromBit),
                            .to = (int32_t)asm_tzcnt(toBit),
                        };
                    }
                    pawnPromotions = asm_blsr(pawnPromotions);
                } while (pawnPromotions);
                score -= 8;
            }
        }
    }

    if (attacked & black.queens) {
        score += 9;
        WHITE_CAPTURE_MOVES_INITIAL(black.queens, false)
        score -= 9;
    }

    if (attacked & black.rooks) {
        score += 5;
        WHITE_CAPTURE_MOVES_INITIAL(black.rooks, false)
        score -= 5;
    }

    if (attacked & black.bishops) {
        score += 3;
        WHITE_CAPTURE_MOVES_INITIAL(black.bishops, false)
        score -= 3;
    }

    if (attacked & black.knights) {
        score += 3;
        WHITE_CAPTURE_MOVES_INITIAL(black.knights, false)
        score -= 3;
    }

    if (attacked & black.pawns) {
        score += 1;
        WHITE_CAPTURE_MOVES_INITIAL(black.pawns, false)
        score -= 1;
    }

    // TODO: What move order for non captures?
    for (uint64_t pawnsForward = white.pawns & ~RANK7 & (~(white.all | black.all) >> 8); pawnsForward != 0; pawnsForward = asm_blsr(pawnsForward)) {
        uint64_t fromBit = asm_blsi(pawnsForward);
        uint64_t toBit = fromBit << 8;
        white.all ^= (fromBit | toBit);
        white.pawns ^= (fromBit | toBit);
        int32_t newScore = evaluateBlackMoves(score, alpha, beta, remainingDepth - 1);
        if (newScore > alpha) {
            alpha = newScore;
            bestMove = (struct move) {
                .from = (int32_t)asm_tzcnt(fromBit),
                .to = (int32_t)asm_tzcnt(toBit),
            };
        }
        white.pawns ^= (fromBit | toBit);
        white.all ^= (fromBit | toBit);
    }

    for (uint64_t knights = white.knights; knights != 0; knights = asm_blsr(knights)) {
        uint64_t from = asm_tzcnt(knights);
        uint64_t moves = gen_knightMoves[from] & ~(white.all | black.all);
        uint64_t fromBit = (uint64_t)1 << from;
        for (; moves != 0; moves = asm_blsr(moves)) {
            uint64_t toBit = asm_blsi(moves);
            white.all ^= (fromBit | toBit);
            white.knights ^= (fromBit | toBit);
            int32_t newScore = evaluateBlackMoves(score, alpha, beta, remainingDepth - 1);
            if (newScore > alpha) {
                alpha = newScore;
                bestMove = (struct move) {
                    .from = (int32_t)from,
                    .to = (int32_t)asm_tzcnt(moves),
                };
            }
            white.knights ^= (fromBit | toBit);
            white.all ^= (fromBit | toBit);
        }
    }

    for (uint64_t bishops = white.bishops; bishops != 0; bishops = asm_blsr(bishops)) {
        uint64_t from = asm_tzcnt(bishops);
        uint64_t fromBit = (uint64_t)1 << from;
        for (uint64_t moves = gen_bishopMoves[(from << 9) | asm_pext(white.all | black.all, gen_bishopMasks[from])] & ~(white.all | black.all); moves != 0; moves = asm_blsr(moves)) {
            uint64_t toBit = asm_blsi(moves);
            white.all ^= (fromBit | toBit);
            white.bishops ^= (fromBit | toBit);
            int32_t newScore = evaluateBlackMoves(score, alpha, beta, remainingDepth - 1);
            if (newScore > alpha) {
                alpha = newScore;
                bestMove = (struct move) {
                    .from = (int32_t)from,
                    .to = (int32_t)asm_tzcnt(moves),
                };
            }
            white.bishops ^= (fromBit | toBit);
            white.all ^= (fromBit | toBit);
        }
    }

    for (uint64_t rooks = white.rooks; rooks != 0; rooks = asm_blsr(rooks)) {
        uint64_t from = asm_tzcnt(rooks);
        uint64_t fromBit = (uint64_t)1 << from;
        for (uint64_t moves = gen_rookMoves[(from << 12) | asm_pext(white.all | black.all, gen_rookMasks[from])] & ~(white.all | black.all); moves != 0; moves = asm_blsr(moves)) {
            uint64_t toBit = asm_blsi(moves);
            white.all ^= (fromBit | toBit);
            white.rooks ^= (fromBit | toBit);
            int32_t newScore = evaluateBlackMoves(score, alpha, beta, remainingDepth - 1);
            if (newScore > alpha) {
                alpha = newScore;
                bestMove = (struct move) {
                    .from = (int32_t)from,
                    .to = (int32_t)asm_tzcnt(moves),
                };
            }
            white.rooks ^= (fromBit | toBit);
            white.all ^= (fromBit | toBit);
        }
    }

    for (uint64_t queens = white.queens; queens != 0; queens = asm_blsr(queens)) {
        uint64_t from = asm_tzcnt(queens);
        uint64_t fromBit = (uint64_t)1 << from;
        for (
            uint64_t moves = (gen_bishopMoves[(from << 9) | asm_pext(white.all | black.all, gen_bishopMasks[from])] | gen_rookMoves[(from << 12) | asm_pext(white.all | black.all, gen_rookMasks[from])]) & ~(white.all | black.all);
            moves != 0;
            moves = asm_blsr(moves)
        ) {
            uint64_t toBit = asm_blsi(moves);
            white.all ^= (fromBit | toBit);
            white.queens ^= (fromBit | toBit);
            int32_t newScore = evaluateBlackMoves(score, alpha, beta, remainingDepth - 1);
            if (newScore > alpha) {
                alpha = newScore;
                bestMove = (struct move) {
                    .from = (int32_t)from,
                    .to = (int32_t)asm_tzcnt(moves),
                };
            }
            white.queens ^= (fromBit | toBit);
            white.all ^= (fromBit | toBit);
        }
    }

    {
        uint64_t from = asm_tzcnt(white.king);
        uint64_t fromBit = (uint64_t)1 << from;
        for (uint64_t moves = gen_kingMoves[from] & ~(white.all | black.all); moves != 0; moves = asm_blsr(moves)) {
            uint64_t toBit = asm_blsi(moves);
            white.all ^= (fromBit | toBit);
            white.king ^= (fromBit | toBit);
            int32_t newScore = evaluateBlackMoves(score, alpha, beta, remainingDepth - 1);
            if (newScore > alpha) {
                alpha = newScore;
                bestMove = (struct move) {
                    .from = (int32_t)from,
                    .to = (int32_t)asm_tzcnt(moves),
                };
            }
            white.king ^= (fromBit | toBit);
            white.all ^= (fromBit | toBit);
        }
    }

    // Done
    if (bestMove.from < 0) return -1;

    debug_printNum("Best move: ", bestMove.from, "->");
    debug_printNum("", bestMove.to, " ");
    debug_printNum("(", alpha, ")\n");

    *moveFrom = bestMove.from;
    *moveTo = bestMove.to;
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