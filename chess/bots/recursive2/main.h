#define FILE_A ((uint64_t)0b0000000100000001000000010000000100000001000000010000000100000001U)
#define FILE_H ((uint64_t)0b1000000010000000100000001000000010000000100000001000000010000000U)
#define RANK2 ((uint64_t)0b0000000000000000000000000000000000000000000000001111111100000000U)
#define RANK7 ((uint64_t)0b0000000011111111000000000000000000000000000000000000000000000000U)
#define INFINITY 8191

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