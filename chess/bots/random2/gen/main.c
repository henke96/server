#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>
#include <string.h>

#define TO_X(INDEX) ((INDEX) % 8)
#define TO_Y(INDEX) ((INDEX) / 8)
#define TO_INDEX(X, Y) ((8 * (Y)) + (X))

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

        fprintf(handle, "%" PRIu64 "U%s", moves, ((i < 63) ? "," : ""));
    }

    if (writeString("};\n") < 0) return -3;
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

        fprintf(handle, "%" PRIu64 "U%s", moves, ((i < 63) ? "," : ""));
    }

    if (writeString("};\n") < 0) return -3;
    return 0;
}

int main(void) {
    handle = fopen("gen.c", "w");
    if (handle == NULL) return 1;
    if (writeKnightMoves() < 0) return 2;
    if (writeKingMoves() < 0) return 3;
    fclose(handle);
    return 0;
}