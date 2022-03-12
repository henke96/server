#include "../../hc/hc.h"
#include "../../hc/libc.h"
#include "../../hc/libc/libc.c"
#include "../../hc/wrappers.c"

int32_t main(hc_UNUSED int32_t argc, hc_UNUSED char **argv) {
    static const char message[7] = "Hello!\n";
    hc_write(STDOUT_FILENO, &message[0], sizeof(message));
    return 0;
}