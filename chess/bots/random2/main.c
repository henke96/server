#include "../../../common/hc/src/hc.h"
#include "../../../common/hc/src/util.c"
#include "../../../common/hc/src/libc/musl.c"
#include "../../../common/hc/src/linux/linux.h"
#include "../../../common/hc/src/linux/sys.c"
#include "../../../common/hc/src/linux/debug.c"
#include "../../../common/hc/src/linux/helpers/_start.c"

#include "../../common/include/protocol.h"
#include "../common/client/include/client.h"

#include "../common/gen/gen.c"
#include "../common/asm.c"
#include "../common/client/src/client.c"

#define FILE_A 0b0000000100000001000000010000000100000001000000010000000100000001U
#define FILE_H 0b1000000010000000100000001000000010000000100000001000000010000000U
#define RANK2 0b0000000000000000000000000000000000000000000000001111111100000000U
#define RANK7 0b0000000011111111000000000000000000000000000000000000000000000000U

struct move {
    int32_t from;
    int32_t to;
};

static uint64_t white;
static uint64_t whitePawns;
static uint64_t whiteKnights;
static uint64_t whiteBishops;
static uint64_t whiteRooks;
static uint64_t whiteQueens;
static uint64_t whiteKing;
static uint64_t black;

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
                }
            } else {
                black |= bit;
            }
        }
    }
}

static int32_t makeMove(bool isHost, uint8_t *board, hc_UNUSED int32_t lastMoveFrom, hc_UNUSED int32_t lastMoveTo, int32_t *moveFrom, int32_t *moveTo) {
    init(isHost, board);

    uint64_t knights = whiteKnights;
    for (; knights != 0; knights = asm_blsr(knights)) {
        uint64_t from = asm_tzcnt(knights);
        uint64_t moves = gen_knightMoves[from] & ~white;
        for (; moves != 0; moves = asm_blsr(moves)) {
            foundMoves[numFoundMoves++] = (struct move) {
                .from = (int32_t)from,
                .to = (int32_t)asm_tzcnt(moves)
            };
        }
    }

    {
        uint64_t from = asm_tzcnt(whiteKing);
        uint64_t moves = gen_kingMoves[from] & ~white;
        for (; moves != 0; moves = asm_blsr(moves)) {
            foundMoves[numFoundMoves++] = (struct move) {
                .from = (int32_t)from,
                .to = (int32_t)asm_tzcnt(moves)
            };
        }
    }

    {
        // TODO: Should occupied be a separate map or just white | black?
        uint64_t pawnsLeft = whitePawns & ~FILE_A & (black >> 7);
        for (; pawnsLeft != 0; pawnsLeft = asm_blsr(pawnsLeft)) {
            int32_t from = (int32_t)asm_tzcnt(pawnsLeft);
            foundMoves[numFoundMoves++] = (struct move) {
                .from = from,
                .to = from + 7
            };
        }

        uint64_t pawnsRight = whitePawns & ~FILE_H & (black >> 9);
        for (; pawnsRight != 0; pawnsRight = asm_blsr(pawnsRight)) {
            int32_t from = (int32_t)asm_tzcnt(pawnsRight);
            foundMoves[numFoundMoves++] = (struct move) {
                .from = from,
                .to = from + 9
            };
        }

        uint64_t pawnsForward = whitePawns & (~(white | black) >> 8);
        for (; pawnsForward != 0; pawnsForward = asm_blsr(pawnsForward)) {
            int32_t from = (int32_t)asm_tzcnt(pawnsForward);
            foundMoves[numFoundMoves++] = (struct move) {
                .from = from,
                .to = from + 8
            };
        }
    }

    uint64_t rooksAndQueens = whiteRooks | whiteQueens;
    for (; rooksAndQueens != 0; rooksAndQueens = asm_blsr(rooksAndQueens)) {
        uint64_t from = asm_tzcnt(rooksAndQueens);
        uint64_t moves = gen_rookMoves[(from << 12) | asm_pext(white | black, gen_rookMasks[from])] & ~white;
        for (; moves != 0; moves = asm_blsr(moves)) {
            foundMoves[numFoundMoves++] = (struct move) {
                .from = (int32_t)from,
                .to = (int32_t)asm_tzcnt(moves)
            };
        }
    }

    uint64_t bishopsAndQueens = whiteBishops | whiteQueens;
    for (; bishopsAndQueens != 0; bishopsAndQueens = asm_blsr(bishopsAndQueens)) {
        uint64_t from = asm_tzcnt(bishopsAndQueens);
        uint64_t moves = gen_bishopMoves[(from << 9) | asm_pext(white | black, gen_bishopMasks[from])] & ~white;
        for (; moves != 0; moves = asm_blsr(moves)) {
            foundMoves[numFoundMoves++] = (struct move) {
                .from = (int32_t)from,
                .to = (int32_t)asm_tzcnt(moves)
            };
        }
    }

    if (numFoundMoves == 0) return -1;
    debug_printNum("Found moves: ", numFoundMoves, "\n");
    int32_t moveIndex;
    sys_getrandom(&moveIndex, 1, GRND_INSECURE);
    moveIndex %= numFoundMoves;
    struct move *move = &foundMoves[moveIndex];
    *moveFrom = move->from;
    *moveTo = move->to;
    return 0;
}

static struct client client;

int32_t main(int32_t argc, char **argv) {
    int32_t roomId = -1;
    if (argc > 1) {
        uint64_t roomIdArg;
        if (util_strToUint(argv[1], '\0', &roomIdArg) == 0 || roomIdArg > INT32_MAX) return 1;
        roomId = (int32_t)roomIdArg;
    }

    client_create(&client, makeMove);
    uint8_t address[] = { 127, 0, 0, 1 };
    int32_t status = client_run(&client, &address[0], 8089, roomId);
    debug_printNum("Status: ", status, "\n");
    return 0;
}