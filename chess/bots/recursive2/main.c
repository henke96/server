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

#define FILE_A 0b0000000100000001000000010000000100000001000000010000000100000001U
#define FILE_H 0b1000000010000000100000001000000010000000100000001000000010000000U
#define RANK2 0b0000000000000000000000000000000000000000000000001111111100000000U
#define RANK7 0b0000000011111111000000000000000000000000000000000000000000000000U

static int32_t evaluateWhiteMoves(int32_t score, int32_t remainingDepth);

struct move {
    int32_t from;
    int32_t to;
    int32_t score;
};

static uint64_t white;
static uint64_t whitePawns;
static uint64_t whiteKnights;
static uint64_t whiteBishops;
static uint64_t whiteRooks;
static uint64_t whiteQueens;
static uint64_t whiteKing;
static uint64_t black;
static uint64_t blackPawns;
static uint64_t blackKnights;
static uint64_t blackBishops;
static uint64_t blackRooks;
static uint64_t blackQueens;
static uint64_t blackKing;

static struct move foundMoves[256];
static int32_t numFoundMoves;

static void init(bool isHost, uint8_t *board) {
    white = 0;
    whitePawns = 0;
    whiteKnights = 0;
    whiteBishops = 0;
    whiteRooks = 0;
    whiteQueens = 0;
    whiteKing = 0;
    black = 0;
    blackPawns = 0;
    blackKnights = 0;
    blackBishops = 0;
    blackRooks = 0;
    blackQueens = 0;
    blackKing = 0;

    numFoundMoves = 0;

    for (int32_t i = 0; i < 64; ++i) {
        uint8_t piece = board[i];
        uint64_t bit = (uint64_t)1 << i;
        if (piece != protocol_NO_PIECE) {
            if (!isHost) piece ^= (protocol_WHITE_FLAG | protocol_BLACK_FLAG);

            if (piece & protocol_WHITE_FLAG) {
                white |= bit;
                switch (piece & protocol_PIECE_MASK) {
                    case protocol_PAWN: {
                        whitePawns |= bit;
                        break;
                    }
                    case protocol_KNIGHT: {
                        whiteKnights |= bit;
                        break;
                    }
                    case protocol_BISHOP: {
                        whiteBishops |= bit;
                        break;
                    }
                    case protocol_ROOK: {
                        whiteRooks |= bit;
                        break;
                    }
                    case protocol_QUEEN: {
                        whiteQueens |= bit;
                        break;
                    }
                    case protocol_KING: {
                        whiteKing |= bit;
                        break;
                    }
                    default: hc_UNREACHABLE;
                }
            } else {
                black |= bit;
                switch (piece & protocol_PIECE_MASK) {
                    case protocol_PAWN: {
                        blackPawns |= bit;
                        break;
                    }
                    case protocol_KNIGHT: {
                        blackKnights |= bit;
                        break;
                    }
                    case protocol_BISHOP: {
                        blackBishops |= bit;
                        break;
                    }
                    case protocol_ROOK: {
                        blackRooks |= bit;
                        break;
                    }
                    case protocol_QUEEN: {
                        blackQueens |= bit;
                        break;
                    }
                    case protocol_KING: {
                        blackKing |= bit;
                        break;
                    }
                    default: hc_UNREACHABLE;
                }
            }
        }
    }
}

#define BLACK_CAPTURE_MOVES(TARGET) \
for (uint64_t PAWNS_LEFT = blackPawns & ~FILE_H & (TARGET << 7); PAWNS_LEFT != 0; PAWNS_LEFT = asm_blsr(PAWNS_LEFT)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_LEFT); \
    uint64_t TO_BIT = (FROM_BIT >> 7); \
    white ^= TO_BIT; \
    TARGET ^= TO_BIT; \
    black ^= (FROM_BIT | TO_BIT); /* TODO: does this optimize well? */ \
    blackPawns ^= (FROM_BIT | TO_BIT); \
    int32_t SCORE = evaluateWhiteMoves(score, remainingDepth - 1); \
    if (SCORE < best) best = SCORE; \
    blackPawns ^= (FROM_BIT | TO_BIT); \
    black ^= (FROM_BIT | TO_BIT); \
    TARGET ^= TO_BIT; \
    white ^= TO_BIT; \
} \
for (uint64_t PAWNS_RIGHT = blackPawns & ~FILE_A & (TARGET << 9); PAWNS_RIGHT != 0; PAWNS_RIGHT = asm_blsr(PAWNS_RIGHT)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_RIGHT); \
    uint64_t TO_BIT = (FROM_BIT >> 9); \
    white ^= TO_BIT; \
    TARGET ^= TO_BIT; \
    black ^= (FROM_BIT | TO_BIT); \
    blackPawns ^= (FROM_BIT | TO_BIT); \
    int32_t SCORE = evaluateWhiteMoves(score, remainingDepth - 1); \
    if (SCORE < best) best = SCORE; \
    blackPawns ^= (FROM_BIT | TO_BIT); \
    black ^= (FROM_BIT | TO_BIT); \
    TARGET ^= TO_BIT; \
    white ^= TO_BIT; \
} \
uint64_t KNIGHTS = blackKnights; \
for (; KNIGHTS != 0; KNIGHTS = asm_blsr(KNIGHTS)) { \
    uint64_t FROM = asm_tzcnt(KNIGHTS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_knightMoves[FROM] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        white ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        black ^= (FROM_BIT | TO_BIT); \
        blackKnights ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = evaluateWhiteMoves(score, remainingDepth - 1); \
        if (SCORE < best) best = SCORE; \
        blackKnights ^= (FROM_BIT | TO_BIT); \
        black ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        white ^= TO_BIT; \
    } \
} \
for (uint64_t BISHOPS = blackBishops; BISHOPS != 0; BISHOPS = asm_blsr(BISHOPS)) { \
    uint64_t FROM = asm_tzcnt(BISHOPS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_bishopMoves[(FROM << 9) | asm_pext(black | white, gen_bishopMasks[FROM])] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        white ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        black ^= (FROM_BIT | TO_BIT); \
        blackBishops ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = evaluateWhiteMoves(score, remainingDepth - 1); \
        if (SCORE < best) best = SCORE; \
        blackBishops ^= (FROM_BIT | TO_BIT); \
        black ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        white ^= TO_BIT; \
    } \
} \
for (uint64_t ROOKS = blackRooks; ROOKS != 0; ROOKS = asm_blsr(ROOKS)) { \
    uint64_t FROM = asm_tzcnt(ROOKS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_rookMoves[(FROM << 12) | asm_pext(black | white, gen_rookMasks[FROM])] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        white ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        black ^= (FROM_BIT | TO_BIT); \
        blackRooks ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = evaluateWhiteMoves(score, remainingDepth - 1); \
        if (SCORE < best) best = SCORE; \
        blackRooks ^= (FROM_BIT | TO_BIT); \
        black ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        white ^= TO_BIT; \
    } \
} \
for (uint64_t QUEENS = blackQueens; QUEENS != 0; QUEENS = asm_blsr(QUEENS)) { \
    uint64_t FROM = asm_tzcnt(QUEENS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for ( \
        uint64_t MOVES = (gen_bishopMoves[(FROM << 9) | asm_pext(black | white, gen_bishopMasks[FROM])] | gen_rookMoves[(FROM << 12) | asm_pext(black | white, gen_rookMasks[FROM])]) & TARGET; \
        MOVES != 0; \
        MOVES = asm_blsr(MOVES) \
    ) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        white ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        black ^= (FROM_BIT | TO_BIT); \
        blackQueens ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = evaluateWhiteMoves(score, remainingDepth - 1); \
        if (SCORE < best) best = SCORE; \
        blackQueens ^= (FROM_BIT | TO_BIT); \
        black ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        white ^= TO_BIT; \
    } \
} \
{ \
    uint64_t FROM = asm_tzcnt(blackKing); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_kingMoves[FROM] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        white ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        black ^= (FROM_BIT | TO_BIT); \
        blackKing ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = evaluateWhiteMoves(score, remainingDepth - 1); \
        if (SCORE < best) best = SCORE; \
        blackKing ^= (FROM_BIT | TO_BIT); \
        black ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        white ^= TO_BIT; \
    } \
}

static int32_t evaluateBlackMoves(int32_t score, int32_t remainingDepth) {
    // Generate all tiles that we attack.
    uint64_t attacked = 0;
    {
        attacked |= ((blackPawns & ~FILE_H) >> 7); // Left
        attacked |= ((blackPawns & ~FILE_A) >> 9); // Right
        attacked |= gen_kingMoves[asm_tzcnt(blackKing)];

        for (uint64_t knights = blackKnights; knights != 0; knights = asm_blsr(knights)) {
            attacked |= gen_knightMoves[asm_tzcnt(knights)];
        }

        for (uint64_t bishopsAndQueens = blackBishops | blackQueens; bishopsAndQueens != 0; bishopsAndQueens = asm_blsr(bishopsAndQueens)) {
            uint64_t from = asm_tzcnt(bishopsAndQueens);
            attacked |= gen_bishopMoves[(from << 9) | asm_pext(black | white, gen_bishopMasks[from])];
        }

        for (uint64_t rooksAndQueens = blackRooks | blackQueens; rooksAndQueens != 0; rooksAndQueens = asm_blsr(rooksAndQueens)) {
            uint64_t from = asm_tzcnt(rooksAndQueens);
            attacked |= gen_rookMoves[(from << 12) | asm_pext(black | white, gen_rookMasks[from])];
        }
    }

    if (attacked & whiteKing) return score - 10000;

    int32_t best = INT32_MAX;

    // TODO: Promotion with capture

    if (attacked & whiteQueens) {
        score -= 9;
        if (remainingDepth == 0) return score;
        BLACK_CAPTURE_MOVES(whiteQueens)
        score += 9;
    }

    // TODO: Promotion without capture (or just pretend its better than queen capture).

    if (attacked & whiteRooks) {
        score -= 5;
        if (remainingDepth == 0) return score;
        BLACK_CAPTURE_MOVES(whiteRooks)
        score += 5;
    }

    if (attacked & whiteBishops) {
        score -= 3;
        if (remainingDepth == 0) return score;
        BLACK_CAPTURE_MOVES(whiteBishops)
        score += 3;
    }

    if (attacked & whiteKnights) {
        score -= 3;
        if (remainingDepth == 0) return score;
        BLACK_CAPTURE_MOVES(whiteKnights)
        score += 3;
    }

    if (attacked & whitePawns) {
        score -= 1;
        if (remainingDepth == 0) return score;
        BLACK_CAPTURE_MOVES(whitePawns)
        score += 1;
    }

    if (remainingDepth == 0) return score;

    // TODO: What move order for non captures?
    for (uint64_t knights = blackKnights; knights != 0; knights = asm_blsr(knights)) {
        uint64_t from = asm_tzcnt(knights);
        uint64_t moves = gen_knightMoves[from] & ~(black | white);
        uint64_t fromBit = (uint64_t)1 << from;
        for (; moves != 0; moves = asm_blsr(moves)) {
            uint64_t toBit = asm_blsi(moves);
            black ^= (fromBit | toBit);
            blackKnights ^= (fromBit | toBit);
            int32_t newScore = evaluateWhiteMoves(score, remainingDepth - 1);
            if (newScore < best) best = newScore;
            blackKnights ^= (fromBit | toBit);
            black ^= (fromBit | toBit);
        }
    }

    {

        for (uint64_t pawnsForward = blackPawns & (~(black | white) << 8); pawnsForward != 0; pawnsForward = asm_blsr(pawnsForward)) {
            uint64_t fromBit = asm_blsi(pawnsForward);
            uint64_t toBit = fromBit << 8;
            black ^= (fromBit | toBit);
            blackPawns ^= (fromBit | toBit);
            int32_t newScore = evaluateWhiteMoves(score, remainingDepth - 1);
            if (newScore < best) best = newScore;
            blackPawns ^= (fromBit | toBit);
            black ^= (fromBit | toBit);
        }
    }

    for (uint64_t bishops = blackBishops; bishops != 0; bishops = asm_blsr(bishops)) {
        uint64_t from = asm_tzcnt(bishops);
        uint64_t fromBit = (uint64_t)1 << from;
        for (uint64_t moves = gen_bishopMoves[(from << 9) | asm_pext(black | white, gen_bishopMasks[from])] & ~(black | white); moves != 0; moves = asm_blsr(moves)) {
            uint64_t toBit = asm_blsi(moves);
            black ^= (fromBit | toBit);
            blackBishops ^= (fromBit | toBit);
            int32_t newScore = evaluateWhiteMoves(score, remainingDepth - 1);
            if (newScore < best) best = newScore;
            blackBishops ^= (fromBit | toBit);
            black ^= (fromBit | toBit);
        }
    }

    for (uint64_t rooks = blackRooks; rooks != 0; rooks = asm_blsr(rooks)) {
        uint64_t from = asm_tzcnt(rooks);
        uint64_t fromBit = (uint64_t)1 << from;
        for (uint64_t moves = gen_rookMoves[(from << 12) | asm_pext(black | white, gen_rookMasks[from])] & ~(black | white); moves != 0; moves = asm_blsr(moves)) {
            uint64_t toBit = asm_blsi(moves);
            black ^= (fromBit | toBit);
            blackRooks ^= (fromBit | toBit);
            int32_t newScore = evaluateWhiteMoves(score, remainingDepth - 1);
            if (newScore < best) best = newScore;
            blackRooks ^= (fromBit | toBit);
            black ^= (fromBit | toBit);
        }
    }

    for (uint64_t queens = blackQueens; queens != 0; queens = asm_blsr(queens)) {
        uint64_t from = asm_tzcnt(queens);
        uint64_t fromBit = (uint64_t)1 << from;
        for (
            uint64_t moves = (gen_bishopMoves[(from << 9) | asm_pext(black | white, gen_bishopMasks[from])] | gen_rookMoves[(from << 12) | asm_pext(black | white, gen_rookMasks[from])]) & ~(black | white);
            moves != 0;
            moves = asm_blsr(moves)
        ) {
            uint64_t toBit = asm_blsi(moves);
            black ^= (fromBit | toBit);
            blackQueens ^= (fromBit | toBit);
            int32_t newScore = evaluateWhiteMoves(score, remainingDepth - 1);
            if (newScore < best) best = newScore;
            blackQueens ^= (fromBit | toBit);
            black ^= (fromBit | toBit);
        }
    }

    {
        uint64_t from = asm_tzcnt(blackKing);
        uint64_t fromBit = (uint64_t)1 << from;
        for (uint64_t moves = gen_kingMoves[from] & ~(black | white); moves != 0; moves = asm_blsr(moves)) {
            uint64_t toBit = asm_blsi(moves);
            black ^= (fromBit | toBit);
            blackKing ^= (fromBit | toBit);
            int32_t newScore = evaluateWhiteMoves(score, remainingDepth - 1);
            if (newScore < best) best = newScore;
            blackKing ^= (fromBit | toBit);
            black ^= (fromBit | toBit);
        }
    }
    return best;
}

#define WHITE_CAPTURE_MOVES(TARGET) \
for (uint64_t PAWNS_LEFT = whitePawns & ~FILE_A & (TARGET >> 7); PAWNS_LEFT != 0; PAWNS_LEFT = asm_blsr(PAWNS_LEFT)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_LEFT); \
    uint64_t TO_BIT = (FROM_BIT << 7); \
    black ^= TO_BIT; \
    TARGET ^= TO_BIT; \
    white ^= (FROM_BIT | TO_BIT); /* TODO: does this optimize well? */ \
    whitePawns ^= (FROM_BIT | TO_BIT); \
    int32_t SCORE = evaluateBlackMoves(score, remainingDepth - 1); \
    if (SCORE > best) best = SCORE; \
    whitePawns ^= (FROM_BIT | TO_BIT); \
    white ^= (FROM_BIT | TO_BIT); \
    TARGET ^= TO_BIT; \
    black ^= TO_BIT; \
} \
for (uint64_t PAWNS_RIGHT = whitePawns & ~FILE_H & (TARGET >> 9); PAWNS_RIGHT != 0; PAWNS_RIGHT = asm_blsr(PAWNS_RIGHT)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_RIGHT); \
    uint64_t TO_BIT = (FROM_BIT << 9); \
    black ^= TO_BIT; \
    TARGET ^= TO_BIT; \
    white ^= (FROM_BIT | TO_BIT); \
    whitePawns ^= (FROM_BIT | TO_BIT); \
    int32_t SCORE = evaluateBlackMoves(score, remainingDepth - 1); \
    if (SCORE > best) best = SCORE; \
    whitePawns ^= (FROM_BIT | TO_BIT); \
    white ^= (FROM_BIT | TO_BIT); \
    TARGET ^= TO_BIT; \
    black ^= TO_BIT; \
} \
uint64_t KNIGHTS = whiteKnights; \
for (; KNIGHTS != 0; KNIGHTS = asm_blsr(KNIGHTS)) { \
    uint64_t FROM = asm_tzcnt(KNIGHTS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_knightMoves[FROM] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        black ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        white ^= (FROM_BIT | TO_BIT); \
        whiteKnights ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = evaluateBlackMoves(score, remainingDepth - 1); \
        if (SCORE > best) best = SCORE; \
        whiteKnights ^= (FROM_BIT | TO_BIT); \
        white ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        black ^= TO_BIT; \
    } \
} \
for (uint64_t BISHOPS = whiteBishops; BISHOPS != 0; BISHOPS = asm_blsr(BISHOPS)) { \
    uint64_t FROM = asm_tzcnt(BISHOPS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_bishopMoves[(FROM << 9) | asm_pext(white | black, gen_bishopMasks[FROM])] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        black ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        white ^= (FROM_BIT | TO_BIT); \
        whiteBishops ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = evaluateBlackMoves(score, remainingDepth - 1); \
        if (SCORE > best) best = SCORE; \
        whiteBishops ^= (FROM_BIT | TO_BIT); \
        white ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        black ^= TO_BIT; \
    } \
} \
for (uint64_t ROOKS = whiteRooks; ROOKS != 0; ROOKS = asm_blsr(ROOKS)) { \
    uint64_t FROM = asm_tzcnt(ROOKS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_rookMoves[(FROM << 12) | asm_pext(white | black, gen_rookMasks[FROM])] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        black ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        white ^= (FROM_BIT | TO_BIT); \
        whiteRooks ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = evaluateBlackMoves(score, remainingDepth - 1); \
        if (SCORE > best) best = SCORE; \
        whiteRooks ^= (FROM_BIT | TO_BIT); \
        white ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        black ^= TO_BIT; \
    } \
} \
for (uint64_t QUEENS = whiteQueens; QUEENS != 0; QUEENS = asm_blsr(QUEENS)) { \
    uint64_t FROM = asm_tzcnt(QUEENS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for ( \
        uint64_t MOVES = (gen_bishopMoves[(FROM << 9) | asm_pext(white | black, gen_bishopMasks[FROM])] | gen_rookMoves[(FROM << 12) | asm_pext(white | black, gen_rookMasks[FROM])]) & TARGET; \
        MOVES != 0; \
        MOVES = asm_blsr(MOVES) \
    ) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        black ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        white ^= (FROM_BIT | TO_BIT); \
        whiteQueens ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = evaluateBlackMoves(score, remainingDepth - 1); \
        if (SCORE > best) best = SCORE; \
        whiteQueens ^= (FROM_BIT | TO_BIT); \
        white ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        black ^= TO_BIT; \
    } \
} \
{ \
    uint64_t FROM = asm_tzcnt(whiteKing); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_kingMoves[FROM] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        black ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        white ^= (FROM_BIT | TO_BIT); \
        whiteKing ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = evaluateBlackMoves(score, remainingDepth - 1); \
        if (SCORE > best) best = SCORE; \
        whiteKing ^= (FROM_BIT | TO_BIT); \
        white ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        black ^= TO_BIT; \
    } \
}

static int32_t evaluateWhiteMoves(int32_t score, int32_t remainingDepth) {
    // Generate all tiles that we attack.
    uint64_t attacked = 0;
    {
        attacked |= ((whitePawns & ~FILE_A) << 7); // Left
        attacked |= ((whitePawns & ~FILE_H) << 9); // Right
        attacked |= gen_kingMoves[asm_tzcnt(whiteKing)];

        for (uint64_t knights = whiteKnights; knights != 0; knights = asm_blsr(knights)) {
            attacked |= gen_knightMoves[asm_tzcnt(knights)];
        }

        for (uint64_t bishopsAndQueens = whiteBishops | whiteQueens; bishopsAndQueens != 0; bishopsAndQueens = asm_blsr(bishopsAndQueens)) {
            uint64_t from = asm_tzcnt(bishopsAndQueens);
            attacked |= gen_bishopMoves[(from << 9) | asm_pext(white | black, gen_bishopMasks[from])];
        }

        for (uint64_t rooksAndQueens = whiteRooks | whiteQueens; rooksAndQueens != 0; rooksAndQueens = asm_blsr(rooksAndQueens)) {
            uint64_t from = asm_tzcnt(rooksAndQueens);
            attacked |= gen_rookMoves[(from << 12) | asm_pext(white | black, gen_rookMasks[from])];
        }
    }

    if (attacked & blackKing) return score + 10000;

    int32_t best = INT32_MIN;

    // TODO: Promotion with capture

    if (attacked & blackQueens) {
        score += 9;
        if (remainingDepth == 0) return score;
        WHITE_CAPTURE_MOVES(blackQueens)
        score -= 9;
    }

    // TODO: Promotion without capture (or just pretend its better than queen capture).

    if (attacked & blackRooks) {
        score += 5;
        if (remainingDepth == 0) return score;
        WHITE_CAPTURE_MOVES(blackRooks)
        score -= 5;
    }

    if (attacked & blackBishops) {
        score += 3;
        if (remainingDepth == 0) return score;
        WHITE_CAPTURE_MOVES(blackBishops)
        score -= 3;
    }

    if (attacked & blackKnights) {
        score += 3;
        if (remainingDepth == 0) return score;
        WHITE_CAPTURE_MOVES(blackKnights)
        score -= 3;
    }

    if (attacked & blackPawns) {
        score += 1;
        if (remainingDepth == 0) return score;
        WHITE_CAPTURE_MOVES(blackPawns)
        score -= 1;
    }

    if (remainingDepth == 0) return score;

    // TODO: What move order for non captures?
    for (uint64_t knights = whiteKnights; knights != 0; knights = asm_blsr(knights)) {
        uint64_t from = asm_tzcnt(knights);
        uint64_t moves = gen_knightMoves[from] & ~(white | black);
        uint64_t fromBit = (uint64_t)1 << from;
        for (; moves != 0; moves = asm_blsr(moves)) {
            uint64_t toBit = asm_blsi(moves);
            white ^= (fromBit | toBit);
            whiteKnights ^= (fromBit | toBit);
            int32_t newScore = evaluateBlackMoves(score, remainingDepth - 1);
            if (newScore > best) best = newScore;
            whiteKnights ^= (fromBit | toBit);
            white ^= (fromBit | toBit);
        }
    }

    {

        for (uint64_t pawnsForward = whitePawns & (~(white | black) >> 8); pawnsForward != 0; pawnsForward = asm_blsr(pawnsForward)) {
            uint64_t fromBit = asm_blsi(pawnsForward);
            uint64_t toBit = fromBit << 8;
            white ^= (fromBit | toBit);
            whitePawns ^= (fromBit | toBit);
            int32_t newScore = evaluateBlackMoves(score, remainingDepth - 1);
            if (newScore > best) best = newScore;
            whitePawns ^= (fromBit | toBit);
            white ^= (fromBit | toBit);
        }
    }

    for (uint64_t bishops = whiteBishops; bishops != 0; bishops = asm_blsr(bishops)) {
        uint64_t from = asm_tzcnt(bishops);
        uint64_t fromBit = (uint64_t)1 << from;
        for (uint64_t moves = gen_bishopMoves[(from << 9) | asm_pext(white | black, gen_bishopMasks[from])] & ~(white | black); moves != 0; moves = asm_blsr(moves)) {
            uint64_t toBit = asm_blsi(moves);
            white ^= (fromBit | toBit);
            whiteBishops ^= (fromBit | toBit);
            int32_t newScore = evaluateBlackMoves(score, remainingDepth - 1);
            if (newScore > best) best = newScore;
            whiteBishops ^= (fromBit | toBit);
            white ^= (fromBit | toBit);
        }
    }

    for (uint64_t rooks = whiteRooks; rooks != 0; rooks = asm_blsr(rooks)) {
        uint64_t from = asm_tzcnt(rooks);
        uint64_t fromBit = (uint64_t)1 << from;
        for (uint64_t moves = gen_rookMoves[(from << 12) | asm_pext(white | black, gen_rookMasks[from])] & ~(white | black); moves != 0; moves = asm_blsr(moves)) {
            uint64_t toBit = asm_blsi(moves);
            white ^= (fromBit | toBit);
            whiteRooks ^= (fromBit | toBit);
            int32_t newScore = evaluateBlackMoves(score, remainingDepth - 1);
            if (newScore > best) best = newScore;
            whiteRooks ^= (fromBit | toBit);
            white ^= (fromBit | toBit);
        }
    }

    for (uint64_t queens = whiteQueens; queens != 0; queens = asm_blsr(queens)) {
        uint64_t from = asm_tzcnt(queens);
        uint64_t fromBit = (uint64_t)1 << from;
        for (
            uint64_t moves = (gen_bishopMoves[(from << 9) | asm_pext(white | black, gen_bishopMasks[from])] | gen_rookMoves[(from << 12) | asm_pext(white | black, gen_rookMasks[from])]) & ~(white | black);
            moves != 0;
            moves = asm_blsr(moves)
        ) {
            uint64_t toBit = asm_blsi(moves);
            white ^= (fromBit | toBit);
            whiteQueens ^= (fromBit | toBit);
            int32_t newScore = evaluateBlackMoves(score, remainingDepth - 1);
            if (newScore > best) best = newScore;
            whiteQueens ^= (fromBit | toBit);
            white ^= (fromBit | toBit);
        }
    }

    {
        uint64_t from = asm_tzcnt(whiteKing);
        uint64_t fromBit = (uint64_t)1 << from;
        for (uint64_t moves = gen_kingMoves[from] & ~(white | black); moves != 0; moves = asm_blsr(moves)) {
            uint64_t toBit = asm_blsi(moves);
            white ^= (fromBit | toBit);
            whiteKing ^= (fromBit | toBit);
            int32_t newScore = evaluateBlackMoves(score, remainingDepth - 1);
            if (newScore > best) best = newScore;
            whiteKing ^= (fromBit | toBit);
            white ^= (fromBit | toBit);
        }
    }
    return best;
}

// Indentical except for the added foundMoves stuff.
#define WHITE_CAPTURE_MOVES_INITIAL(TARGET, IS_KING) \
for (uint64_t PAWNS_LEFT = whitePawns & ~FILE_A & (TARGET >> 7); PAWNS_LEFT != 0; PAWNS_LEFT = asm_blsr(PAWNS_LEFT)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_LEFT); \
    uint64_t TO_BIT = (FROM_BIT << 7); \
    black ^= TO_BIT; \
    TARGET ^= TO_BIT; \
    white ^= (FROM_BIT | TO_BIT); /* TODO: does this optimize well? */ \
    whitePawns ^= (FROM_BIT | TO_BIT); \
    int32_t SCORE = score; \
    if (!IS_KING) SCORE = evaluateBlackMoves(score, remainingDepth - 1); \
    foundMoves[numFoundMoves++] = (struct move) { \
        .from = (int32_t)asm_tzcnt(FROM_BIT), \
        .to = (int32_t)asm_tzcnt(TO_BIT), \
        .score = SCORE \
    }; \
    if (SCORE > best) best = SCORE; \
    whitePawns ^= (FROM_BIT | TO_BIT); \
    white ^= (FROM_BIT | TO_BIT); \
    TARGET ^= TO_BIT; \
    black ^= TO_BIT; \
} \
for (uint64_t PAWNS_RIGHT = whitePawns & ~FILE_H & (TARGET >> 9); PAWNS_RIGHT != 0; PAWNS_RIGHT = asm_blsr(PAWNS_RIGHT)) { \
    uint64_t FROM_BIT = asm_blsi(PAWNS_RIGHT); \
    uint64_t TO_BIT = (FROM_BIT << 9); \
    black ^= TO_BIT; \
    TARGET ^= TO_BIT; \
    white ^= (FROM_BIT | TO_BIT); \
    whitePawns ^= (FROM_BIT | TO_BIT); \
    int32_t SCORE = score; \
    if (!IS_KING) SCORE = evaluateBlackMoves(score, remainingDepth - 1); \
    foundMoves[numFoundMoves++] = (struct move) { \
        .from = (int32_t)asm_tzcnt(FROM_BIT), \
        .to = (int32_t)asm_tzcnt(TO_BIT), \
        .score = SCORE \
    }; \
    if (SCORE > best) best = SCORE; \
    whitePawns ^= (FROM_BIT | TO_BIT); \
    white ^= (FROM_BIT | TO_BIT); \
    TARGET ^= TO_BIT; \
    black ^= TO_BIT; \
} \
uint64_t KNIGHTS = whiteKnights; \
for (; KNIGHTS != 0; KNIGHTS = asm_blsr(KNIGHTS)) { \
    uint64_t FROM = asm_tzcnt(KNIGHTS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_knightMoves[FROM] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        black ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        white ^= (FROM_BIT | TO_BIT); \
        whiteKnights ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = score; \
        if (!IS_KING) SCORE = evaluateBlackMoves(score, remainingDepth - 1); \
        foundMoves[numFoundMoves++] = (struct move) { \
            .from = (int32_t)FROM, \
            .to = (int32_t)asm_tzcnt(MOVES), \
            .score = SCORE \
        }; \
        if (SCORE > best) best = SCORE; \
        whiteKnights ^= (FROM_BIT | TO_BIT); \
        white ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        black ^= TO_BIT; \
    } \
} \
for (uint64_t BISHOPS = whiteBishops; BISHOPS != 0; BISHOPS = asm_blsr(BISHOPS)) { \
    uint64_t FROM = asm_tzcnt(BISHOPS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_bishopMoves[(FROM << 9) | asm_pext(white | black, gen_bishopMasks[FROM])] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        black ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        white ^= (FROM_BIT | TO_BIT); \
        whiteBishops ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = score; \
        if (!IS_KING) SCORE = evaluateBlackMoves(score, remainingDepth - 1); \
        foundMoves[numFoundMoves++] = (struct move) { \
            .from = (int32_t)FROM, \
            .to = (int32_t)asm_tzcnt(MOVES), \
            .score = SCORE \
        }; \
        if (SCORE > best) best = SCORE; \
        whiteBishops ^= (FROM_BIT | TO_BIT); \
        white ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        black ^= TO_BIT; \
    } \
} \
for (uint64_t ROOKS = whiteRooks; ROOKS != 0; ROOKS = asm_blsr(ROOKS)) { \
    uint64_t FROM = asm_tzcnt(ROOKS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_rookMoves[(FROM << 12) | asm_pext(white | black, gen_rookMasks[FROM])] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        black ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        white ^= (FROM_BIT | TO_BIT); \
        whiteRooks ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = score; \
        if (!IS_KING) SCORE = evaluateBlackMoves(score, remainingDepth - 1); \
        foundMoves[numFoundMoves++] = (struct move) { \
            .from = (int32_t)FROM, \
            .to = (int32_t)asm_tzcnt(MOVES), \
            .score = SCORE \
        }; \
        if (SCORE > best) best = SCORE; \
        whiteRooks ^= (FROM_BIT | TO_BIT); \
        white ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        black ^= TO_BIT; \
    } \
} \
for (uint64_t QUEENS = whiteQueens; QUEENS != 0; QUEENS = asm_blsr(QUEENS)) { \
    uint64_t FROM = asm_tzcnt(QUEENS); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for ( \
        uint64_t MOVES = (gen_bishopMoves[(FROM << 9) | asm_pext(white | black, gen_bishopMasks[FROM])] | gen_rookMoves[(FROM << 12) | asm_pext(white | black, gen_rookMasks[FROM])]) & TARGET; \
        MOVES != 0; \
        MOVES = asm_blsr(MOVES) \
    ) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        black ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        white ^= (FROM_BIT | TO_BIT); \
        whiteQueens ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = score; \
        if (!IS_KING) SCORE = evaluateBlackMoves(score, remainingDepth - 1); \
        foundMoves[numFoundMoves++] = (struct move) { \
            .from = (int32_t)FROM, \
            .to = (int32_t)asm_tzcnt(MOVES), \
            .score = SCORE \
        }; \
        if (SCORE > best) best = SCORE; \
        whiteQueens ^= (FROM_BIT | TO_BIT); \
        white ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        black ^= TO_BIT; \
    } \
} \
{ \
    uint64_t FROM = asm_tzcnt(whiteKing); \
    uint64_t FROM_BIT = (uint64_t)1 << FROM; \
    for (uint64_t MOVES = gen_kingMoves[FROM] & TARGET; MOVES != 0; MOVES = asm_blsr(MOVES)) { \
        uint64_t TO_BIT = asm_blsi(MOVES); \
        black ^= TO_BIT; \
        TARGET ^= TO_BIT; \
        white ^= (FROM_BIT | TO_BIT); \
        whiteKing ^= (FROM_BIT | TO_BIT); \
        int32_t SCORE = score; \
        if (!IS_KING) SCORE = evaluateBlackMoves(score, remainingDepth - 1); \
        foundMoves[numFoundMoves++] = (struct move) { \
            .from = (int32_t)FROM, \
            .to = (int32_t)asm_tzcnt(MOVES), \
            .score = SCORE \
        }; \
        if (SCORE > best) best = SCORE; \
        whiteKing ^= (FROM_BIT | TO_BIT); \
        white ^= (FROM_BIT | TO_BIT); \
        TARGET ^= TO_BIT; \
        black ^= TO_BIT; \
    } \
}

static int32_t makeMove(bool isHost, uint8_t *board, hc_UNUSED int32_t lastMoveFrom, hc_UNUSED int32_t lastMoveTo, int32_t *moveFrom, int32_t *moveTo) {
    init(isHost, board);
    int32_t remainingDepth = 6;
    int32_t score = 0;

    // Generate all tiles that we attack.
    uint64_t attacked = 0;
    {
        attacked |= ((whitePawns & ~FILE_A) << 7); // Left
        attacked |= ((whitePawns & ~FILE_H) << 9); // Right
        attacked |= gen_kingMoves[asm_tzcnt(whiteKing)];

        for (uint64_t knights = whiteKnights; knights != 0; knights = asm_blsr(knights)) {
            attacked |= gen_knightMoves[asm_tzcnt(knights)];
        }

        for (uint64_t bishopsAndQueens = whiteBishops | whiteQueens; bishopsAndQueens != 0; bishopsAndQueens = asm_blsr(bishopsAndQueens)) {
            uint64_t from = asm_tzcnt(bishopsAndQueens);
            attacked |= gen_bishopMoves[(from << 9) | asm_pext(white | black, gen_bishopMasks[from])];
        }

        for (uint64_t rooksAndQueens = whiteRooks | whiteQueens; rooksAndQueens != 0; rooksAndQueens = asm_blsr(rooksAndQueens)) {
            uint64_t from = asm_tzcnt(rooksAndQueens);
            attacked |= gen_rookMoves[(from << 12) | asm_pext(white | black, gen_rookMasks[from])];
        }
    }

    int32_t best = INT32_MIN;

    if (attacked & blackKing) {
        score += 10000;
        WHITE_CAPTURE_MOVES_INITIAL(blackKing, true)
        score -= 10000;
    }

    // TODO: Promotion with capture

    if (attacked & blackQueens) {
        score += 9;
        WHITE_CAPTURE_MOVES_INITIAL(blackQueens, false)
        score -= 9;
    }

    // TODO: Promotion without capture (or just pretend its better than queen capture).

    if (attacked & blackRooks) {
        score += 5;
        WHITE_CAPTURE_MOVES_INITIAL(blackRooks, false)
        score -= 5;
    }

    if (attacked & blackBishops) {
        score += 3;
        WHITE_CAPTURE_MOVES_INITIAL(blackBishops, false)
        score -= 3;
    }

    if (attacked & blackKnights) {
        score += 3;
        WHITE_CAPTURE_MOVES_INITIAL(blackKnights, false)
        score -= 3;
    }

    if (attacked & blackPawns) {
        score += 1;
        WHITE_CAPTURE_MOVES_INITIAL(blackPawns, false)
        score -= 1;
    }

    // TODO: What move order for non captures?
    for (uint64_t knights = whiteKnights; knights != 0; knights = asm_blsr(knights)) {
        uint64_t from = asm_tzcnt(knights);
        uint64_t moves = gen_knightMoves[from] & ~(white | black);
        uint64_t fromBit = (uint64_t)1 << from;
        for (; moves != 0; moves = asm_blsr(moves)) {
            uint64_t toBit = asm_blsi(moves);
            white ^= (fromBit | toBit);
            whiteKnights ^= (fromBit | toBit);
            int32_t newScore = evaluateBlackMoves(score, remainingDepth - 1);
            foundMoves[numFoundMoves++] = (struct move) { \
                .from = (int32_t)from,
                .to = (int32_t)asm_tzcnt(moves),
                .score = newScore
            };
            if (newScore > best) best = newScore;
            whiteKnights ^= (fromBit | toBit);
            white ^= (fromBit | toBit);
        }
    }

    {

        for (uint64_t pawnsForward = whitePawns & (~(white | black) >> 8); pawnsForward != 0; pawnsForward = asm_blsr(pawnsForward)) {
            uint64_t fromBit = asm_blsi(pawnsForward);
            uint64_t toBit = fromBit << 8;
            white ^= (fromBit | toBit);
            whitePawns ^= (fromBit | toBit);
            int32_t newScore = evaluateBlackMoves(score, remainingDepth - 1);
            foundMoves[numFoundMoves++] = (struct move) { \
                .from = (int32_t)asm_tzcnt(fromBit),
                .to = (int32_t)asm_tzcnt(toBit),
                .score = newScore
            };
            if (newScore > best) best = newScore;
            whitePawns ^= (fromBit | toBit);
            white ^= (fromBit | toBit);
        }
    }

    for (uint64_t bishops = whiteBishops; bishops != 0; bishops = asm_blsr(bishops)) {
        uint64_t from = asm_tzcnt(bishops);
        uint64_t fromBit = (uint64_t)1 << from;
        for (uint64_t moves = gen_bishopMoves[(from << 9) | asm_pext(white | black, gen_bishopMasks[from])] & ~(white | black); moves != 0; moves = asm_blsr(moves)) {
            uint64_t toBit = asm_blsi(moves);
            white ^= (fromBit | toBit);
            whiteBishops ^= (fromBit | toBit);
            int32_t newScore = evaluateBlackMoves(score, remainingDepth - 1);
            foundMoves[numFoundMoves++] = (struct move) { \
                .from = (int32_t)from,
                .to = (int32_t)asm_tzcnt(moves),
                .score = newScore
            };
            if (newScore > best) best = newScore;
            whiteBishops ^= (fromBit | toBit);
            white ^= (fromBit | toBit);
        }
    }

    for (uint64_t rooks = whiteRooks; rooks != 0; rooks = asm_blsr(rooks)) {
        uint64_t from = asm_tzcnt(rooks);
        uint64_t fromBit = (uint64_t)1 << from;
        for (uint64_t moves = gen_rookMoves[(from << 12) | asm_pext(white | black, gen_rookMasks[from])] & ~(white | black); moves != 0; moves = asm_blsr(moves)) {
            uint64_t toBit = asm_blsi(moves);
            white ^= (fromBit | toBit);
            whiteRooks ^= (fromBit | toBit);
            int32_t newScore = evaluateBlackMoves(score, remainingDepth - 1);
            foundMoves[numFoundMoves++] = (struct move) { \
                .from = (int32_t)from,
                .to = (int32_t)asm_tzcnt(moves),
                .score = newScore
            };
            if (newScore > best) best = newScore;
            whiteRooks ^= (fromBit | toBit);
            white ^= (fromBit | toBit);
        }
    }

    for (uint64_t queens = whiteQueens; queens != 0; queens = asm_blsr(queens)) {
        uint64_t from = asm_tzcnt(queens);
        uint64_t fromBit = (uint64_t)1 << from;
        for (
            uint64_t moves = (gen_bishopMoves[(from << 9) | asm_pext(white | black, gen_bishopMasks[from])] | gen_rookMoves[(from << 12) | asm_pext(white | black, gen_rookMasks[from])]) & ~(white | black);
            moves != 0;
            moves = asm_blsr(moves)
        ) {
            uint64_t toBit = asm_blsi(moves);
            white ^= (fromBit | toBit);
            whiteQueens ^= (fromBit | toBit);
            int32_t newScore = evaluateBlackMoves(score, remainingDepth - 1);
            foundMoves[numFoundMoves++] = (struct move) { \
                .from = (int32_t)from,
                .to = (int32_t)asm_tzcnt(moves),
                .score = newScore
            };
            if (newScore > best) best = newScore;
            whiteQueens ^= (fromBit | toBit);
            white ^= (fromBit | toBit);
        }
    }

    {
        uint64_t from = asm_tzcnt(whiteKing);
        uint64_t fromBit = (uint64_t)1 << from;
        for (uint64_t moves = gen_kingMoves[from] & ~(white | black); moves != 0; moves = asm_blsr(moves)) {
            uint64_t toBit = asm_blsi(moves);
            white ^= (fromBit | toBit);
            whiteKing ^= (fromBit | toBit);
            int32_t newScore = evaluateBlackMoves(score, remainingDepth - 1);
            foundMoves[numFoundMoves++] = (struct move) { \
                .from = (int32_t)from,
                .to = (int32_t)asm_tzcnt(moves),
                .score = newScore
            };
            if (newScore > best) best = newScore;
            whiteKing ^= (fromBit | toBit);
            white ^= (fromBit | toBit);
        }
    }

    // Done
    if (numFoundMoves == 0) return -1;

    int32_t numBestMoves = 0;
    for (int32_t i = 0; i < numFoundMoves; ++i) {
        if (foundMoves[i].score == best) {
            ++numBestMoves;
            debug_printNum("Best move: ", foundMoves[i].from, "->");
            debug_printNum("", foundMoves[i].to, " ");
            debug_printNum("(", best, ")\n");
        }
    }

    int32_t moveIndex;
    hc_getrandom(&moveIndex, 1, GRND_INSECURE);
    moveIndex %= numBestMoves;
    for (int32_t i = 0, j = 0; i < numFoundMoves; ++i) {
        if (foundMoves[i].score == best) {
            if (j == moveIndex) {
                *moveFrom = foundMoves[i].from;
                *moveTo = foundMoves[i].to;
                return 0;
            }
            ++j;
        }
    }
    return -1;
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