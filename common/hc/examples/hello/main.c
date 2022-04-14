#include "../../src/hc.h"
#include "../../src/linux/linux.h"
#include "../../src/linux/sys.c"
#include "../../src/linux/helpers/_start.c"

int32_t main(hc_UNUSED int32_t argc, hc_UNUSED char **argv) {
    sys_write(STDOUT_FILENO, "Hello!\n", 7);
    return 0;
}