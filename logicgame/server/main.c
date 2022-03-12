#include "../../common/hc/hc/hc.h"
#include "../../common/hc/hc/libc.h"
#include "../../common/hc/hc/libc/libc.c"
#include "../../common/hc/hc/wrappers.c"
#include "../../common/hc/hc/libhc/util.c"
#include "../../common/hc/hc/libhc/debug.c"

#include "client/generatedHtml.h"
#include "../../common/server/include/base64.h"
#include "../../common/server/include/fileResponse.h"
#include "../../common/server/include/serverCallbacks.h"
#include "../../common/server/include/serverClient.h"
#include "../../common/server/include/server.h"
#include "include/protocol.h"
#include "include/client.h"
#include "include/game.h"

#include "src/client.c"
#include "src/game.c"
#include "../../common/server/src/server.c"
#include "../../common/server/src/serverCallbacks.c"
#include "../../common/server/src/serverClient.c"
#include "../../common/server/src/fileResponse.c"
#include "../../common/server/src/base64.c"

static struct game game;

int32_t main(hc_UNUSED int32_t argc, hc_UNUSED char **argv) {
    int32_t status = game_init(&game);
    if (status < 0) {
        debug_printNum("Failed to initialize logic game (", status, ")\n");
        return 1;
    }

    status = game_run(&game);
    if (status < 0) {
        debug_printNum("Logic game ran into error (", status, ")\n");
        status = 1;
        goto cleanup_game;
    }
    status = 0;
    cleanup_game:
    game_deinit(&game);
    return status;
}