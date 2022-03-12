#include "../../../common/hc/hc/hc.h"
#include "../../../common/hc/hc/libc.h"
#include "../../../common/hc/hc/libc/libc.c"
#include "../../../common/hc/hc/wrappers.c"
#include "../../../common/hc/hc/libhc/util.c"
#include "../../../common/hc/hc/libhc/debug.c"

#include "../../common/include/timespec.h"
#include "../../common/include/protocol.h"
#include "../common/client/include/client.h"

#include "../common/common.c"
#include "../common/client/src/client.c"
#include "recursive.c"

static struct client client;

int32_t main(int32_t argc, char **argv) {
    int32_t roomId = -1;
    if (argc > 1) {
        uint64_t roomIdArg;
        if (util_strToUint(argv[1], '\0', &roomIdArg) == 0 || roomIdArg > INT32_MAX) return 1;
        roomId = (int32_t)roomIdArg;
    }

    client_create(&client, recursive_makeMove);
    uint8_t address[] = {
        127, 0, 0, 1
    };
    int32_t status = client_run(&client, &address[0], 8089, roomId);
    debug_printNum("Status: ", status, "\n");
    return 0;
}
