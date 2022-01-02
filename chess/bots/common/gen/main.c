#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>
#include <string.h>

#define TO_X(INDEX) ((INDEX) % 8)
#define TO_Y(INDEX) ((INDEX) / 8)
#define TO_INDEX(X, Y) ((8 * (Y)) + (X))
#define TO_BIT(X, Y) ((uint64_t)1 << (TO_INDEX((X), (Y))))

#define TRY_ADD_MOVE(X, Y) \
    if ((X) >= 0 && (X) < 8 && (Y) >= 0 && (Y) < 8) { \
        moves |= ((uint64_t)1 << TO_INDEX(X, Y)); \
    }

static FILE *handle;

static int32_t writeString(const char *str) {
    size_t len = strlen(str);
    if (fwrite(str, 1, len, handle) != len) return -1;
    return 0;
}

static int32_t popcount(uint64_t x) {
    int32_t count = 0;
    while (x != 0) {
        if ((x & 1) != 0) ++count;
        x >>= 1;
    }
    return count;
}

uint64_t pdep(uint64_t x, uint64_t mask) {
    uint64_t result = 0;
    uint64_t xBit = 1;

    for (int32_t i = 0; i < 64; ++i) {
        uint64_t maskBit = (uint64_t)1 << i;
        if (mask & maskBit) {
            if (x & xBit) result |= maskBit;
            xBit <<= 1;
        }
    }

    return result;
}

static int32_t writeKnightMoves(void) {
    if (writeString("uint64_t gen_knightMoves[]={") < 0) return -1;

    for (int32_t i = 0; i < 64; ++i) {
        int32_t x = TO_X(i);
        int32_t y = TO_Y(i);
        uint64_t moves = 0;
        TRY_ADD_MOVE(x - 2, y - 1)
        TRY_ADD_MOVE(x + 2, y - 1)
        TRY_ADD_MOVE(x - 2, y + 1)
        TRY_ADD_MOVE(x + 2, y + 1)
        TRY_ADD_MOVE(x - 1, y - 2)
        TRY_ADD_MOVE(x - 1, y + 2)
        TRY_ADD_MOVE(x + 1, y - 2)
        TRY_ADD_MOVE(x + 1, y + 2)

        fprintf(handle, "0x%" PRIx64 "U,", moves);
    }

    if (writeString("0};\n") < 0) return -2;

    // Lookup moves for 0, 1 or 2 knights at once.
    // A lookup is done with gen_multiKnightMoves[(asm_lzcnt(knights) << 6) | asm_tzcnt(knights)].
    // Note that both tzcnt(0) and lzcnt(0) is 64, so the table must have a zero at index ((64 << 6) | 64) = 4160.
    if (writeString("uint64_t gen_multiKnightMoves[]={") < 0) return -3;

    for (int32_t lzCount = 0; lzCount < 64; ++lzCount) {
        for (int32_t tzCount = 0; tzCount < 64; ++tzCount) {
            uint64_t moves = 0;
            int32_t xs[2] = {TO_X(tzCount), TO_X(63 - lzCount)};
            int32_t ys[2] = {TO_Y(tzCount), TO_Y(63 - lzCount)};
            for (int32_t i = 0; i < 2; ++i) {
                int32_t x = xs[i];
                int32_t y = ys[i];
                TRY_ADD_MOVE(x - 2, y - 1)
                TRY_ADD_MOVE(x + 2, y - 1)
                TRY_ADD_MOVE(x - 2, y + 1)
                TRY_ADD_MOVE(x + 2, y + 1)
                TRY_ADD_MOVE(x - 1, y - 2)
                TRY_ADD_MOVE(x - 1, y + 2)
                TRY_ADD_MOVE(x + 1, y - 2)
                TRY_ADD_MOVE(x + 1, y + 2)
            }

            fprintf(handle, "0x%" PRIx64 "U,", moves);
        }
    }

    // Fill end with zeroes.
    int32_t last = ((64 << 6) | 64);
    for (int32_t i = (64 << 6); i <= last; ++i) {
        fprintf(handle, "0%s", ((i < last) ? "," : ""));
    }

    if (writeString("};\n") < 0) return -4;
    return 0;
}

static int32_t writeKingMoves(void) {
    if (writeString("uint64_t gen_kingMoves[]={") < 0) return -1;

    for (int32_t i = 0; i < 64; ++i) {
        int32_t x = TO_X(i);
        int32_t y = TO_Y(i);
        uint64_t moves = 0;
        TRY_ADD_MOVE(x - 1, y - 1)
        TRY_ADD_MOVE(x    , y - 1)
        TRY_ADD_MOVE(x + 1, y - 1)
        TRY_ADD_MOVE(x - 1, y)
        TRY_ADD_MOVE(x + 1, y)
        TRY_ADD_MOVE(x - 1, y + 1)
        TRY_ADD_MOVE(x    , y + 1)
        TRY_ADD_MOVE(x + 1, y + 1)

        fprintf(handle, "0x%" PRIx64 "U%s", moves, ((i < 63) ? "," : ""));
    }

    if (writeString("};\n") < 0) return -2;
    return 0;
}

static int32_t writeRookMoves(void) {
    // gen_rookMoves[] is a lookup of tile and occupied tiles in "the slide area except for the end tiles".
    // Worst case there can be 12 such tiles, and 2^12 = 4096, so the full table ends up at 64 * 4096 = 262114 (2 MiB).
    // A lookup is done with gen_rookMoves[(index << 12) | asm_pext(white | black, gen_rookMasks[index])].

    uint64_t rookMasks[64] = { 0 };

    // Generate and write masks.
    if (writeString("uint64_t gen_rookMasks[]={") < 0) return -1;
    for (int32_t i = 0; i < 64; ++i) {
        int32_t startX = TO_X(i);
        int32_t startY = TO_Y(i);

        for (int32_t x = startX + 1; x < 7; ++x) {
            rookMasks[i] |= TO_BIT(x, startY);
        }
        for (int32_t x = startX - 1; x > 0; --x) {
            rookMasks[i] |= TO_BIT(x, startY);
        }
        for (int32_t y = startY + 1; y < 7; ++y) {
            rookMasks[i] |= TO_BIT(startX, y);
        }
        for (int32_t y = startY - 1; y > 0; --y) {
            rookMasks[i] |= TO_BIT(startX, y);
        }
        fprintf(handle, "0x%" PRIx64 "U,", rookMasks[i]);
    }
    if (writeString("0};\n") < 0) return -2;

    // Generate and write moves.
    if (writeString("uint64_t gen_rookMoves[]={") < 0) return -3;
    for (int32_t i = 0; i < 64; ++i) {
        uint64_t numPossibleOccupied = (uint64_t)1 << popcount(rookMasks[i]);
        for (uint64_t occupied = 0; occupied < 4096; ++occupied) {
            uint64_t moves = 0;

            if (occupied < numPossibleOccupied) {
                uint64_t actualOccupied = pdep(occupied, rookMasks[i]);
                int32_t startX = TO_X(i);
                int32_t startY = TO_Y(i);
                for (int32_t x = startX + 1; x < 8; ++x) {
                    moves |= TO_BIT(x, startY);
                    if (actualOccupied & TO_BIT(x, startY)) break;
                }
                for (int32_t x = startX - 1; x >= 0; --x) {
                    moves |= TO_BIT(x, startY);
                    if (actualOccupied & TO_BIT(x, startY)) break;
                }
                for (int32_t y = startY + 1; y < 8; ++y) {
                    moves |= TO_BIT(startX, y);
                    if (actualOccupied & TO_BIT(startX, y)) break;
                }
                for (int32_t y = startY - 1; y >= 0; --y) {
                    moves |= TO_BIT(startX, y);
                    if (actualOccupied & TO_BIT(startX, y)) break;
                }
            }
            fprintf(handle, "0x%" PRIx64 "U,", moves);
        }
    }
    if (writeString("0};\n") < 0) return -4;

    return 0;
}

static int32_t writeBishopMoves(void) {
    // For bishops the worst case off occupied possibilities is 9 tiles, and 2^9 = 512.
    uint64_t bishopMasks[64] = { 0 };

    // Generate and write masks.
    if (writeString("uint64_t gen_bishopMasks[]={") < 0) return -1;
    for (int32_t i = 0; i < 64; ++i) {
        int32_t startX = TO_X(i);
        int32_t startY = TO_Y(i);

        for (int32_t x = startX + 1, y = startY + 1; x < 7 && y < 7; ++x, ++y) {
            bishopMasks[i] |= TO_BIT(x, y);
        }
        for (int32_t x = startX + 1, y = startY - 1; x < 7 && y > 0; ++x, --y) {
            bishopMasks[i] |= TO_BIT(x, y);
        }
        for (int32_t x = startX - 1, y = startY + 1; x > 0 && y < 7; --x, ++y) {
            bishopMasks[i] |= TO_BIT(x, y);
        }
        for (int32_t x = startX - 1, y = startY - 1; x > 0 && y > 0; --x, --y) {
            bishopMasks[i] |= TO_BIT(x, y);
        }
        fprintf(handle, "0x%" PRIx64 "U,", bishopMasks[i]);
    }
    if (writeString("0};\n") < 0) return -2;

    // Generate and write moves.
    if (writeString("uint64_t gen_bishopMoves[]={") < 0) return -3;
    for (int32_t i = 0; i < 64; ++i) {
        uint64_t numPossibleOccupied = (uint64_t)1 << popcount(bishopMasks[i]);
        for (uint64_t occupied = 0; occupied < 512; ++occupied) {
            uint64_t moves = 0;

            if (occupied < numPossibleOccupied) {
                uint64_t actualOccupied = pdep(occupied, bishopMasks[i]);
                int32_t startX = TO_X(i);
                int32_t startY = TO_Y(i);

                for (int32_t x = startX + 1, y = startY + 1; x < 8 && y < 8; ++x, ++y) {
                    moves |= TO_BIT(x, y);
                    if (actualOccupied & TO_BIT(x, y)) break;
                }
                for (int32_t x = startX + 1, y = startY - 1; x < 8 && y >= 0; ++x, --y) {
                    moves |= TO_BIT(x, y);
                    if (actualOccupied & TO_BIT(x, y)) break;
                }
                for (int32_t x = startX - 1, y = startY + 1; x >= 0 && y < 8; --x, ++y) {
                    moves |= TO_BIT(x, y);
                    if (actualOccupied & TO_BIT(x, y)) break;
                }
                for (int32_t x = startX - 1, y = startY - 1; x >= 0 && y >= 0; --x, --y) {
                    moves |= TO_BIT(x, y);
                    if (actualOccupied & TO_BIT(x, y)) break;
                }
            }
            fprintf(handle, "0x%" PRIx64 "U,", moves);
        }
    }
    if (writeString("0};\n") < 0) return -4;

    return 0;
}

int main(void) {
    handle = fopen("gen.c", "w");
    if (handle == NULL) return 1;
    if (writeKnightMoves() < 0) return 2;
    if (writeKingMoves() < 0) return 3;
    if (writeRookMoves() < 0) return 4;
    if (writeBishopMoves() < 0) return 5;
    fclose(handle);
    return 0;
}