static int32_t recursive_evaluateWhiteMove(int32_t whitePiece, int32_t to, int32_t remainingDepth, int32_t score, int32_t alpha, int32_t beta);

#define recursive_DEPTH 5

static const int32_t recursive_pieceValues[7] = {
    0, // No piece
    1, // Pawn
    3, // Bishop
    3, // Knight
    5, // Rook
    9, // Queen
};

static int32_t whitePieces[16];
static int32_t whiteNumPieces;
static int32_t blackPieces[16];
static int32_t blackNumPieces;

static int32_t pieceIndices[144];

static struct common common;

static void recursive_initPieces(void) {
    memset(&pieceIndices, 0, sizeof(pieceIndices));
    whiteNumPieces = 0;
    blackNumPieces = 0;
    for (int32_t i = 0; i < 64; ++i) {
        int32_t index = common_CONVERT_INDEX(i);
        uint8_t piece = common.board[index];
        if ((piece & protocol_WHITE_FLAG) != 0) {
            pieceIndices[index] = whiteNumPieces;
            whitePieces[whiteNumPieces++] = index;
        } else if ((piece & protocol_BLACK_FLAG) != 0) {
            pieceIndices[index] = blackNumPieces;
            blackPieces[blackNumPieces++] = index;
        }
    }
}

#define recursive_TRY_WHITE_MOVE \
    if ((common.board[testIndex] & protocol_WHITE_FLAG) == 0) { \
        int32_t SCORE = recursive_evaluateWhiteMove(i, testIndex, remainingDepth - 1, score, alpha, beta); \
        if (SCORE > best) { \
            best = SCORE; \
            if (SCORE > alpha) { \
                if (SCORE >= beta) goto breakSearch; \
                alpha = SCORE; \
            } \
        } \
    }

#define recursive_TRY_WHITE_MOVES(OFFSET) \
    for (testIndex = index + OFFSET; (common.board[testIndex] & protocol_WHITE_FLAG) == 0; testIndex += OFFSET) { \
        int32_t SCORE = recursive_evaluateWhiteMove(i, testIndex, remainingDepth - 1, score, alpha, beta); \
        if (SCORE > best) { \
            best = SCORE; \
            if (SCORE > alpha) { \
                if (SCORE >= beta) goto breakSearch; \
                alpha = SCORE; \
            } \
        } \
        if (common.board[testIndex] != 0) break; \
    }

#define recursive_TRY_BLACK_MOVE \
    if ((common.board[testIndex] & protocol_BLACK_FLAG) == 0) { \
        int32_t SCORE = recursive_evaluateBlackMove(i, testIndex, remainingDepth - 1, score, alpha, beta); \
        if (SCORE < best) { \
            best = SCORE; \
            if (SCORE < beta) { \
                if (SCORE <= alpha) goto breakSearch; \
                beta = SCORE; \
            } \
        } \
    }

#define recursive_TRY_BLACK_MOVES(OFFSET) \
    for (testIndex = index + OFFSET; (common.board[testIndex] & protocol_BLACK_FLAG) == 0; testIndex += OFFSET) { \
        int32_t SCORE = recursive_evaluateBlackMove(i, testIndex, remainingDepth - 1, score, alpha, beta); \
        if (SCORE < best) { \
            best = SCORE; \
            if (SCORE < beta) { \
                if (SCORE <= alpha) goto breakSearch; \
                beta = SCORE; \
            } \
        } \
        if (common.board[testIndex] != 0) break; \
    }

#define UP 12
#define DOWN (-12)
#define RIGHT 1
#define LEFT (-1)

static int32_t recursive_evaluateBlackMove(int32_t blackPiece, int32_t to, int32_t remainingDepth, int32_t score, int32_t alpha, int32_t beta) {
    int32_t from = blackPieces[blackPiece];

    uint8_t takenPiece = common.board[to];
    if (takenPiece == (protocol_KING | protocol_WHITE_FLAG)) return score - 10000;

    score -= recursive_pieceValues[takenPiece & protocol_PIECE_MASK];
    uint8_t originalPiece = common.board[from];
    if (originalPiece == (protocol_PAWN | protocol_BLACK_FLAG) && to < common_CONVERT_INDEX(8)) {
        score -= 8;
        if (remainingDepth == 0) return score;
        common.board[from] = protocol_QUEEN | protocol_BLACK_FLAG;
    } else if (remainingDepth == 0) return score;

    int32_t pieceIndex = pieceIndices[to];
    if (takenPiece != 0) {
        debug_ASSERT(pieceIndex < whiteNumPieces);
        whitePieces[pieceIndex] = whitePieces[--whiteNumPieces];
        pieceIndices[whitePieces[pieceIndex]] = pieceIndex;
    }

    // Find best response for white.
    common.board[to] = common.board[from];
    common.board[from] = 0;
    pieceIndices[to] = blackPiece;
    blackPieces[blackPiece] = to;
    int32_t best = INT32_MIN;
    for (int32_t i = 0; i < whiteNumPieces; ++i) {
        int32_t index = whitePieces[i];
        uint8_t piece = common.board[index];
        debug_ASSERT(piece & protocol_WHITE_FLAG);

        int32_t testIndex;
        switch (piece & protocol_PIECE_MASK) {
            case protocol_KING: {
                testIndex = index + DOWN + LEFT;
                recursive_TRY_WHITE_MOVE
                testIndex = index + DOWN;
                recursive_TRY_WHITE_MOVE
                testIndex = index + DOWN + RIGHT;
                recursive_TRY_WHITE_MOVE
                testIndex = index + LEFT;
                recursive_TRY_WHITE_MOVE
                testIndex = index + RIGHT;
                recursive_TRY_WHITE_MOVE
                testIndex = index + UP + LEFT;
                recursive_TRY_WHITE_MOVE
                testIndex = index + UP;
                recursive_TRY_WHITE_MOVE
                testIndex = index + UP + RIGHT;
                recursive_TRY_WHITE_MOVE
                break;
            }
            case protocol_QUEEN:
            case protocol_BISHOP: {
                recursive_TRY_WHITE_MOVES(UP + LEFT)
                recursive_TRY_WHITE_MOVES(DOWN + LEFT)
                recursive_TRY_WHITE_MOVES(UP + RIGHT)
                recursive_TRY_WHITE_MOVES(DOWN + RIGHT)
                if ((piece & protocol_PIECE_MASK) == protocol_BISHOP) break;
                hc_FALLTHROUGH;
            }
            case protocol_ROOK: {
                recursive_TRY_WHITE_MOVES(UP)
                recursive_TRY_WHITE_MOVES(DOWN)
                recursive_TRY_WHITE_MOVES(RIGHT)
                recursive_TRY_WHITE_MOVES(LEFT)
                break;
            }
            case protocol_KNIGHT: {
                testIndex = index + 2 * LEFT + DOWN;
                recursive_TRY_WHITE_MOVE
                testIndex = index + 2 * RIGHT + DOWN;
                recursive_TRY_WHITE_MOVE
                testIndex = index + 2 * RIGHT + UP;
                recursive_TRY_WHITE_MOVE
                testIndex = index + 2 * LEFT + UP;
                recursive_TRY_WHITE_MOVE
                testIndex = index + 2 * DOWN + LEFT;
                recursive_TRY_WHITE_MOVE
                testIndex = index + 2 * DOWN + RIGHT;
                recursive_TRY_WHITE_MOVE
                testIndex = index + 2 * UP + RIGHT;
                recursive_TRY_WHITE_MOVE
                testIndex = index + 2 * UP + LEFT;
                recursive_TRY_WHITE_MOVE
                break;
            }
            case protocol_PAWN: {
                testIndex = index + UP;
                if (common.board[testIndex] == 0) {
                    int32_t moveScore = recursive_evaluateWhiteMove(i, testIndex, remainingDepth - 1, score, alpha, beta);
                    if (moveScore > best) {
                        best = moveScore;
                        if (moveScore > alpha) {
                            if (moveScore >= beta) goto breakSearch;
                            alpha = moveScore;
                        }
                    }
                }

                testIndex = index + UP + LEFT;
                if (common.board[testIndex] != 0 && (common.board[testIndex] & protocol_WHITE_FLAG) == 0) {
                    int32_t moveScore = recursive_evaluateWhiteMove(i, testIndex, remainingDepth - 1, score, alpha, beta);
                    if (moveScore > best) {
                        best = moveScore;
                        if (moveScore > alpha) {
                            if (moveScore >= beta) goto breakSearch;
                            alpha = moveScore;
                        }
                    }
                }

                testIndex = index + UP + RIGHT;
                if (common.board[testIndex] != 0 && (common.board[testIndex] & protocol_WHITE_FLAG) == 0) {
                    int32_t moveScore = recursive_evaluateWhiteMove(i, testIndex, remainingDepth - 1, score, alpha, beta);
                    if (moveScore > best) {
                        best = moveScore;
                        if (moveScore > alpha) {
                            if (moveScore >= beta) goto breakSearch;
                            alpha = moveScore;
                        }
                    }
                }
                break;
            }
            default: hc_UNREACHABLE;
        }
    }
    breakSearch:
    if (takenPiece != 0) {
        pieceIndices[whitePieces[pieceIndex]] = whiteNumPieces;
        whitePieces[whiteNumPieces++] = whitePieces[pieceIndex];
        whitePieces[pieceIndex] = to;
    }
    common.board[to] = takenPiece;
    pieceIndices[to] = pieceIndex;
    common.board[from] = originalPiece;
    blackPieces[blackPiece] = from;
    return best;
}

static int32_t recursive_evaluateWhiteMove(int32_t whitePiece, int32_t to, int32_t remainingDepth, int32_t score, int32_t alpha, int32_t beta) {
    int32_t from = whitePieces[whitePiece];

    uint8_t takenPiece = common.board[to];
    if (takenPiece == (protocol_KING | protocol_BLACK_FLAG)) return score + 10000;

    score += recursive_pieceValues[takenPiece & protocol_PIECE_MASK];
    uint8_t originalPiece = common.board[from];
    if (originalPiece == (protocol_PAWN | protocol_WHITE_FLAG) && to >= common_CONVERT_INDEX(56)) {
        score += 8;
        if (remainingDepth == 0) return score;
        common.board[from] = protocol_QUEEN | protocol_WHITE_FLAG;
    } else if (remainingDepth == 0) return score;

    int32_t pieceIndex = pieceIndices[to];
    if (takenPiece != 0) {
        debug_ASSERT(pieceIndex < blackNumPieces);
        blackPieces[pieceIndex] = blackPieces[--blackNumPieces];
        pieceIndices[blackPieces[pieceIndex]] = pieceIndex;
    }

    // Find best response for black.
    common.board[to] = common.board[from];
    common.board[from] = 0;
    pieceIndices[to] = whitePiece;
    whitePieces[whitePiece] = to;
    int32_t best = INT32_MAX;
    for (int32_t i = 0; i < blackNumPieces; ++i) {
        int32_t index = blackPieces[i];
        int32_t piece = common.board[index];
        debug_ASSERT(piece & protocol_BLACK_FLAG);

        int32_t testIndex;
        switch (piece & protocol_PIECE_MASK) {
            case protocol_KING: {
                testIndex = index + DOWN + LEFT;
                recursive_TRY_BLACK_MOVE
                testIndex = index + DOWN;
                recursive_TRY_BLACK_MOVE
                testIndex = index + DOWN + RIGHT;
                recursive_TRY_BLACK_MOVE
                testIndex = index + LEFT;
                recursive_TRY_BLACK_MOVE
                testIndex = index + RIGHT;
                recursive_TRY_BLACK_MOVE
                testIndex = index + UP + LEFT;
                recursive_TRY_BLACK_MOVE
                testIndex = index + UP;
                recursive_TRY_BLACK_MOVE
                testIndex = index + UP + RIGHT;
                recursive_TRY_BLACK_MOVE
                break;
            }
            case protocol_QUEEN:
            case protocol_BISHOP: {
                recursive_TRY_BLACK_MOVES(UP + LEFT)
                recursive_TRY_BLACK_MOVES(DOWN + LEFT)
                recursive_TRY_BLACK_MOVES(UP + RIGHT)
                recursive_TRY_BLACK_MOVES(DOWN + RIGHT)
                if ((piece & protocol_PIECE_MASK) == protocol_BISHOP) break;
                hc_FALLTHROUGH;
            }
            case protocol_ROOK: {
                recursive_TRY_BLACK_MOVES(UP)
                recursive_TRY_BLACK_MOVES(DOWN)
                recursive_TRY_BLACK_MOVES(RIGHT)
                recursive_TRY_BLACK_MOVES(LEFT)
                break;
            }
            case protocol_KNIGHT: {
                testIndex = index + 2 * LEFT + DOWN;
                recursive_TRY_BLACK_MOVE
                testIndex = index + 2 * RIGHT + DOWN;
                recursive_TRY_BLACK_MOVE
                testIndex = index + 2 * RIGHT + UP;
                recursive_TRY_BLACK_MOVE
                testIndex = index + 2 * LEFT + UP;
                recursive_TRY_BLACK_MOVE
                testIndex = index + 2 * DOWN + LEFT;
                recursive_TRY_BLACK_MOVE
                testIndex = index + 2 * DOWN + RIGHT;
                recursive_TRY_BLACK_MOVE
                testIndex = index + 2 * UP + RIGHT;
                recursive_TRY_BLACK_MOVE
                testIndex = index + 2 * UP + LEFT;
                recursive_TRY_BLACK_MOVE
                break;
            }
            case protocol_PAWN: {
                testIndex = index + DOWN;
                if (common.board[testIndex] == 0) {
                    int32_t moveScore = recursive_evaluateBlackMove(i, testIndex, remainingDepth - 1, score, alpha, beta);
                    if (moveScore < best) {
                        best = moveScore;
                        if (moveScore < beta) {
                            if (moveScore <= alpha) goto breakSearch;
                            beta = moveScore;
                        }
                    }
                }

                testIndex = index + DOWN + LEFT;
                if (common.board[testIndex] != 0 && (common.board[testIndex] & protocol_BLACK_FLAG) == 0) {
                    int32_t moveScore = recursive_evaluateBlackMove(i, testIndex, remainingDepth - 1, score, alpha, beta);
                    if (moveScore < best) {
                        best = moveScore;
                        if (moveScore < beta) {
                            if (moveScore <= alpha) goto breakSearch;
                            beta = moveScore;
                        }
                    }
                }

                testIndex = index + DOWN + RIGHT;
                if (common.board[testIndex] != 0 && (common.board[testIndex] & protocol_BLACK_FLAG) == 0) {
                    int32_t moveScore = recursive_evaluateBlackMove(i, testIndex, remainingDepth - 1, score, alpha, beta);
                    if (moveScore < best) {
                        best = moveScore;
                        if (moveScore < beta) {
                            if (moveScore <= alpha) goto breakSearch;
                            beta = moveScore;
                        }
                    }
                }
                break;
            }
            default: hc_UNREACHABLE;
        }
    }
    breakSearch:
    if (takenPiece != 0) {
        pieceIndices[blackPieces[pieceIndex]] = blackNumPieces;
        blackPieces[blackNumPieces++] = blackPieces[pieceIndex];
        blackPieces[pieceIndex] = to;
    }
    common.board[to] = takenPiece;
    pieceIndices[to] = pieceIndex;
    common.board[from] = originalPiece;
    whitePieces[whitePiece] = from;
    return best;
}

#undef UP
#undef DOWN
#undef RIGHT
#undef LEFT

static int32_t recursive_makeMove(bool isHost, uint8_t *board, hc_UNUSED int32_t lastMoveFrom, hc_UNUSED int32_t lastMoveTo, int32_t *moveFrom, int32_t *moveTo) {
    common_init(&common, isHost, board);
    recursive_initPieces();
    common_dumpBoard(&common);
    common_findMoves(&common);

    debug_printNum("Found ", common.numMoves, " moves\n");
    if (common.numMoves == 0) return -1;

    int32_t scores[common_MAX_MOVES];

    int32_t bestScore = INT32_MIN;
    int32_t numBestMoves = 0;
    for (int32_t i = 0; i < common.numMoves; ++i) {
        scores[i] = recursive_evaluateWhiteMove(pieceIndices[common.moves[i].from], common.moves[i].to, recursive_DEPTH, 0, INT32_MIN, INT32_MAX);
        if (scores[i] > bestScore) {
            bestScore = scores[i];
        }
    }

    for (int32_t i = 0; i < common.numMoves; ++i) {
        if (scores[i] == bestScore) {
            ++numBestMoves;
            debug_printNum("Best move: ", common_CONVERT_INDEX_BACK(common.moves[i].from), "->");
            debug_printNum("", common_CONVERT_INDEX_BACK(common.moves[i].to), " ");
            debug_printNum("(", bestScore, ")\n");
        }
    }

    int32_t moveIndex;
    hc_getrandom(&moveIndex, 1, GRND_INSECURE);
    moveIndex %= numBestMoves;
    for (int32_t i = 0, j = 0; i < common.numMoves; ++i) {
        if (scores[i] == bestScore) {
            if (j == moveIndex) {
                *moveFrom = common_CONVERT_INDEX_BACK(common.moves[i].from);
                *moveTo = common_CONVERT_INDEX_BACK(common.moves[i].to);
                return 0;
            }
            ++j;
        }
    }
    return -1;
}
