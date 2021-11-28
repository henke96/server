#include "../../../common/linux/hc/hc.h"
#include "../../../common/linux/hc/libc.h"
#include "../../../common/linux/hc/libc.c"
#include "../../../common/linux/hc/wrappers.c"
#include "../../../common/linux/debug.c"

static hc_ALWAYS_INLINE uint64_t tzcnt(uint64_t x) {
    uint64_t result;
    asm("tzcnt %1, %0\n"
        : "=r"(result)
        : "r"(x)
    );
    return result;
}

static hc_ALWAYS_INLINE uint64_t popcnt(uint64_t x) {
    uint64_t result;
    asm("popcnt %1, %0\n"
        : "=r"(result)
        : "r"(x)
    );
    return result;
}

static hc_ALWAYS_INLINE uint64_t blsr(uint64_t x) {
    uint64_t result;
    asm("blsr %1, %0\n"
        : "=r"(result)
        : "r"(x)
    );
    return result;
}

static hc_ALWAYS_INLINE uint64_t blsi(uint64_t x) {
    uint64_t result;
    asm("blsi %1, %0\n"
        : "=r"(result)
        : "r"(x)
    );
    return result;
}

static hc_ALWAYS_INLINE uint64_t pext(uint64_t x, uint64_t mask) {
    uint64_t result;
    asm("pext %2, %1, %0\n"
        : "=r"(result)
        : "r"(x), "r"(mask)
    );
    return result;
}

int32_t main(int32_t argc, hc_UNUSED char **argv) {
    uint64_t num = (uint64_t)argc;
    debug_printNum("popcnt: ", (int64_t)popcnt(num), "\n");
    debug_printNum("tzcnt: ", (int64_t)tzcnt(num), "\n");
    debug_printNum("blsi: ", (int64_t)blsi(num), "\n");
    num = blsr(num);
    debug_printNum("popcnt: ", (int64_t)popcnt(num), "\n");
    debug_printNum("tzcnt: ", (int64_t)tzcnt(num), "\n");
    debug_printNum("blsi: ", (int64_t)blsi(num), "\n");

    uint64_t x =    0b1000000000000000000000000000000000000000000000000010000000000000;
    uint64_t mask = 0b1000000000000000000000000000000000000000000000000010001000000001;
    debug_printNum("pext: ", (int64_t)pext(x, mask), "\n");

    return 0;
}