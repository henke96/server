#define common_MAX_MOVES 256
#define common_CONVERT_INDEX(INDEX) (INDEX + 26 + 4 * (INDEX / 8))
#define common_CONVERT_INDEX_BACK(INDEX) (INDEX - 18 - 4 * (INDEX / 12))

struct common_move {
    int32_t from;
    int32_t to;
};

struct common {
    uint8_t board[144]; // 12 * 12
    struct common_move moves[common_MAX_MOVES];
    int32_t numMoves;
};

static void common_init(struct common *self, bool isHost, uint8_t *board) {
    // Make borders
    for (int32_t i = 0; i < 12; ++i) {
        // Bottom
        self->board[i] = protocol_WHITE_FLAG | protocol_BLACK_FLAG;
        self->board[12 + i] = protocol_WHITE_FLAG  | protocol_BLACK_FLAG;
        // Top
        self->board[12 * 10 + i] = protocol_WHITE_FLAG | protocol_BLACK_FLAG;
        self->board[12 * 11 + i] = protocol_WHITE_FLAG | protocol_BLACK_FLAG;
        // Left
        self->board[i * 12] = protocol_WHITE_FLAG | protocol_BLACK_FLAG;
        self->board[i * 12 + 1] = protocol_WHITE_FLAG | protocol_BLACK_FLAG;
        // Right
        self->board[i * 12 + 10] = protocol_WHITE_FLAG | protocol_BLACK_FLAG;
        self->board[i * 12 + 11] = protocol_WHITE_FLAG | protocol_BLACK_FLAG;
    }

    for (int32_t i = 0; i < 64; ++i) {
        uint8_t piece = board[i];
        if (!isHost && piece != protocol_NO_PIECE) {
            piece ^= (protocol_WHITE_FLAG | protocol_BLACK_FLAG);
        }
        self->board[common_CONVERT_INDEX(i)] = piece;
    }
}

static void common_dumpBoard(struct common *self) {
    for (int32_t y = 11; y >= 0; --y) {
        for (int32_t x = 0; x < 12; ++x) {
            uint8_t piece = self->board[y * 12 + x];
            char symbol;
            switch (piece & protocol_PIECE_MASK) {
                case protocol_KING:   symbol = 'k'; break;
                case protocol_QUEEN:  symbol = 'q'; break;
                case protocol_BISHOP: symbol = 'b'; break;
                case protocol_KNIGHT: symbol = 'n'; break;
                case protocol_ROOK:   symbol = 'r'; break;
                case protocol_PAWN:   symbol = 'p'; break;
                case protocol_NO_PIECE: {
                    if (piece & protocol_WHITE_FLAG) symbol = 'X';
                    else symbol = ' ';
                    goto done;
                }
                default: hc_UNREACHABLE;
            }
            if (piece & protocol_WHITE_FLAG) {
                symbol = (char)(symbol - 32);
            }
            done:
            hc_write(STDOUT_FILENO, &symbol, 1);
        }
        hc_write(STDOUT_FILENO, "\n", 1);
    }
}

#define common_TRY_MOVE(OFFSET) \
    testIndex = index + (OFFSET); \
    if ((self->board[testIndex] & protocol_WHITE_FLAG) == 0) { \
        self->moves[self->numMoves++] = (struct common_move) { \
            .from = index, \
            .to = testIndex \
        }; \
    }

#define common_TRY_MOVES(OFFSET) \
    for (testIndex = index + (OFFSET); (self->board[testIndex] & protocol_WHITE_FLAG) == 0; testIndex += (OFFSET)) { \
        self->moves[self->numMoves++] = (struct common_move) { \
            .from = index, \
            .to = testIndex \
        }; \
        if (self->board[testIndex] != 0) break; \
    }

#define UP 12
#define DOWN (-12)
#define RIGHT 1
#define LEFT (-1)

static void common_findMoves(struct common *self) {
    self->numMoves = 0;
    for (int32_t i = 0; i < 64; ++i) {
        int32_t index = common_CONVERT_INDEX(i);
        if (self->board[index] & protocol_WHITE_FLAG) {
            uint8_t piece = self->board[index] & protocol_PIECE_MASK;
            int32_t testIndex;
            switch (piece) {
                case protocol_KING: {
                    common_TRY_MOVE(DOWN + LEFT)
                    common_TRY_MOVE(DOWN)
                    common_TRY_MOVE(DOWN + RIGHT)
                    common_TRY_MOVE(LEFT)
                    common_TRY_MOVE(RIGHT)
                    common_TRY_MOVE(UP + LEFT)
                    common_TRY_MOVE(UP)
                    common_TRY_MOVE(UP + RIGHT)
                    break;
                }
                case protocol_QUEEN:
                case protocol_BISHOP: {
                    common_TRY_MOVES(UP + LEFT)
                    common_TRY_MOVES(DOWN + LEFT)
                    common_TRY_MOVES(UP + RIGHT)
                    common_TRY_MOVES(DOWN + RIGHT)
                    if (piece == protocol_BISHOP) break;
                    hc_FALLTHROUGH;
                }
                case protocol_ROOK: {
                    common_TRY_MOVES(UP)
                    common_TRY_MOVES(DOWN)
                    common_TRY_MOVES(RIGHT)
                    common_TRY_MOVES(LEFT)
                    break;
                }
                case protocol_KNIGHT: {
                    common_TRY_MOVE(2 * LEFT + DOWN)
                    common_TRY_MOVE(2 * RIGHT + DOWN)
                    common_TRY_MOVE(2 * RIGHT + UP)
                    common_TRY_MOVE(2 * LEFT + UP)
                    common_TRY_MOVE(2 * DOWN + LEFT)
                    common_TRY_MOVE(2 * DOWN + RIGHT)
                    common_TRY_MOVE(2 * UP + RIGHT)
                    common_TRY_MOVE(2 * UP + LEFT)
                    break;
                }
                case protocol_PAWN: {
                    testIndex = index + UP;
                    if (self->board[testIndex] == 0) {
                        self->moves[self->numMoves++] = (struct common_move) {
                            .from = index,
                            .to = testIndex
                        };
                    }

                    testIndex = index + UP + LEFT;
                    if (self->board[testIndex] != 0 && (self->board[testIndex] & protocol_WHITE_FLAG) == 0) {
                        self->moves[self->numMoves++] = (struct common_move) {
                            .from = index,
                            .to = testIndex
                        };
                    }

                    testIndex = index + UP + RIGHT;
                    if (self->board[testIndex] != 0 && (self->board[testIndex] & protocol_WHITE_FLAG) == 0) {
                        self->moves[self->numMoves++] = (struct common_move) {
                            .from = index,
                            .to = testIndex
                        };
                    }
                    break;
                }
                default: hc_UNREACHABLE;
            }
        }
    }
}

#undef UP
#undef DOWN
#undef RIGHT
#undef LEFT
