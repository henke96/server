#include "../../src/hc.h"
#include "../../src/wasi/wasi.h"

void noreturn _start(void) {
    struct ciovec iov = {
        .buffer = (uint8_t *)"Hello!\n",
        .bufferLength = 7
    };
    uint32_t ret;
    fd_write(1, &iov, 1, &ret);
    proc_exit(ret);
}