#define BOARD(X, Y) board[63 - ((Y - 1) * 8 + (X - 1))]
#define WHITE(PIECE) (PIECE | protocol_WHITE_FLAG)
#define BLACK(PIECE) (PIECE | protocol_BLACK_FLAG)

#define TIME(FUNCTION) \
    { \
        clock_gettime(CLOCK_MONOTONIC, &startTime); \
        int STATUS = FUNCTION(true, &board[0], 18, 26, &moveFrom, &moveTo); \
        if (STATUS < 0) { \
            printf("%s failed to make move\n", #FUNCTION); \
            return -1; \
        } \
        clock_gettime(CLOCK_MONOTONIC, &endTime); \
        int64_t delta = timespec_toNanoseconds(endTime) - timespec_toNanoseconds(startTime); \
        printf("%s took: %" PRId64 ".%09" PRId64 "\n", #FUNCTION, delta / 1000000000, delta % 1000000000); \
    }


static uint8_t board[64];

static int parseBoard(char *board) {
    int32_t y = 7;
    int32_t x = 0;
    while (*board != '\0' && y >= 0) {
        int32_t i = y * 8 + x;
        if (i < 0) return -1;
        switch (*board) {
            case 'p': board[i] = protocol_PAWN | protocol_BLACK_FLAG; break;
            case 'r': board[i] = protocol_ROOK | protocol_BLACK_FLAG; break;
            case 'n': board[i] = protocol_KNIGHT | protocol_BLACK_FLAG; break;
            case 'b': board[i] = protocol_BISHOP | protocol_BLACK_FLAG; break;
            case 'q': board[i] = protocol_QUEEN | protocol_BLACK_FLAG; break;
            case 'k': board[i] = protocol_KING | protocol_BLACK_FLAG; break;

            case 'P': board[i] = protocol_PAWN | protocol_WHITE_FLAG; break;
            case 'R': board[i] = protocol_ROOK | protocol_WHITE_FLAG; break;
            case 'N': board[i] = protocol_KNIGHT | protocol_WHITE_FLAG; break;
            case 'B': board[i] = protocol_BISHOP | protocol_WHITE_FLAG; break;
            case 'Q': board[i] = protocol_QUEEN | protocol_WHITE_FLAG; break;
            case 'K': board[i] = protocol_KING | protocol_WHITE_FLAG; break;
            case ' ': break;
            default: --x;
        }
        ++x;
        if (x > 7) {
            x = 0;
            --y;
        }
        ++board;
    }
    return 0;
}

int main(int argc, char **argv) {
    if (parseBoard("\
XXXXXXXXXXXX\
XXXXXXXXXXXX\
XXr k qbnrXX\
XXpbp   ppXX\
XX p  p   XX\
XX   pnp  XX\
XX P     QXX\
XX    PP  XX\
XXP PP  PPXX\
XXRNBK BNRXX\
XXXXXXXXXXXX\
XXXXXXXXXXXX") < 0) return 1;

    struct timespec startTime;
    struct timespec endTime;
    int32_t moveFrom;
    int32_t moveTo;

    TIME(recursive_makeMove)
    return 0;
}

#undef BOARD
#undef WHITE
#undef BLACK
#undef TIME
