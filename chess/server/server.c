#ifndef UNREACHABLE
#define UNREACHABLE assert(0)
#endif

#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>

#include <sched.h>
#include <sys/prctl.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>

// linux/if_alg.h
struct sockaddr_alg {
    uint16_t salg_family;
    uint8_t salg_type[14];
    uint32_t salg_feat;
    uint32_t salg_mask;
    uint8_t salg_name[64];
};

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

#include "src/main.c"
#include "src/chess.c"
#include "src/chessRoom.c"
#include "src/chessClient.c"
#include "../../common/server/src/server.c"
#include "../../common/server/src/serverCallbacks.c"
#include "../../common/server/src/serverClient.c"
#include "../../common/server/src/fileResponse.c"
#include "../../common/server/src/base64.c"
