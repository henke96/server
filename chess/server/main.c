#include "../../common/hc/hc/hc.h"
#include "../../common/hc/hc/libc.h"
#include "../../common/hc/hc/libc/libc.c"
#include "../../common/hc/hc/wrappers.c"
#include "../../common/hc/hc/libhc/allocator.c"
#include "../../common/hc/hc/libhc/debug.c"

#include "client/generatedHtml.h"
#include "../common/include/protocol.h"
#include "../common/include/timespec.h"
#include "../../common/server/include/base64.h"
#include "../../common/server/include/fileResponse.h"
#include "../../common/server/include/serverCallbacks.h"
#include "../../common/server/include/serverClient.h"
#include "../../common/server/include/server.h"
#include "include/chessClient.h"
#include "include/chessRoom.h"
#include "include/chess.h"

#include "src/chess.c"
#include "src/chessRoom.c"
#include "src/chessClient.c"
#include "../../common/server/src/server.c"
#include "../../common/server/src/serverCallbacks.c"
#include "../../common/server/src/serverClient.c"
#include "../../common/server/src/fileResponse.c"
#include "../../common/server/src/base64.c"

static struct chess chess;

int32_t main(hc_UNUSED int32_t argc, hc_UNUSED char **argv) {
    int32_t status = chess_init(&chess);
    if (status < 0) {
        debug_printNum("Failed to initialize chess (", status, ")\n");
        return 1;
    }

    status = chess_run(&chess);
    if (status < 0) {
        debug_printNum("Chess ran into error (", status, ")\n");
        status = 1;
        goto cleanup_chess;
    }
    status = 0;
    cleanup_chess:
    chess_deinit(&chess);
    return status;
}