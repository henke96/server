#include "../../hc/hc.h"
#include "../../hc/libc.h"
#include "../../hc/libc/libc.c"
#include "../../hc/wrappers.c"

int32_t main(hc_UNUSED int32_t argc, hc_UNUSED char **argv) {
    hc_write(STDOUT_FILENO, "Hello!\n", 7);
    return 0;
}