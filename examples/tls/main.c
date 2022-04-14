#include "../../src/hc.h"
#include "../../src/elf.h"
#include "../../src/util.c"
#include "../../src/libc/small.c"
#include "../../src/linux/linux.h"
#include "../../src/linux/util.c"
#include "../../src/linux/sys.c"
#include "../../src/linux/tls.c"
#include "../../src/linux/debug.c"
#include "../../src/linux/helpers/_start.c"
#include "../../src/linux/helpers/sys_clone.c"

static thread_local int32_t test1 = 1;
static thread_local char test2 = 2;
static thread_local int64_t test3 = 3;
static thread_local int16_t test4;
static thread_local int32_t test5;
static thread_local int64_t test6 = 6;

// For the main thread we use this array as tls area.
static char tlsArea[tls_AREA_SIZE(64)] hc_ALIGNED(8);

#define CHILD_NOT_DONE 0
#define CHILD_DONE 1
static int32_t childDone = CHILD_NOT_DONE;

static noreturn void thread(void *arg) {
    test4 = -4;
    test5 = -5;
    test6 = (int64_t)arg;

    debug_printNum("child test1 = ", test1, "\n");
    debug_printNum("child test2 = ", test2, "\n");
    debug_printNum("child test3 = ", test3, "\n");
    debug_printNum("child test4 = ", test4, "\n");
    debug_printNum("child test5 = ", test5, "\n");
    debug_printNum("child test6 = ", test6, "\n");
    debug_printNum("child &test1 = ", (int64_t)&test1, "\n");
    debug_printNum("child &test2 = ", (int64_t)&test2, "\n");
    debug_printNum("child &test3 = ", (int64_t)&test3, "\n");
    debug_printNum("child &test4 = ", (int64_t)&test4, "\n");
    debug_printNum("child &test5 = ", (int64_t)&test5, "\n");
    debug_printNum("child &test6 = ", (int64_t)&test6, "\n");

    // Notify parent we are done.
    hc_ATOMIC_STORE(&childDone, CHILD_DONE, hc_ATOMIC_RELEASE);
    int32_t hc_UNUSED status = sys_futex(&childDone, FUTEX_WAKE_PRIVATE, 1, NULL, NULL, 0);
    debug_ASSERT(status >= 0);

    sys_exit(0);
}

int32_t main(int32_t argc, char **argv) {
    uint64_t *auxv = util_getAuxv(util_getEnvp(argc, argv));

    struct elf_programHeader *tlsProgramHeader = tls_findProgramHeader(auxv);
    if (tlsProgramHeader == NULL) return 1;

    // Setup tls for main thread.
    {
        uint64_t threadPointer = tls_initArea(tlsProgramHeader, &tlsArea[0]);
        tls_setThreadPointer(threadPointer);
        debug_printNum("parent thread pointer: ", (int64_t)threadPointer, "\n");
        test4 = 4;
        test5 = 5;
    }

    // Start child thread with tls.
    {
        // Allocate stack and tls area for child thread.
        int64_t stackSize = 4096; // Tls area must be aligned to at least `tlsProgramHeader->segmentAlignment`,
                                  // so we make the stack size a nice power of 2 at put the tls area right after it.
        int64_t allocSize = stackSize + (int64_t)tls_AREA_SIZE(tlsProgramHeader->segmentMemorySize);
        char *threadArea = sys_mmap(NULL, allocSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
        if ((int64_t)threadArea < 0) return 2;

        uint64_t threadPointer = tls_initArea(tlsProgramHeader, &threadArea[stackSize]);
        debug_printNum("child thread pointer: ", (int64_t)threadPointer, "\n");
        struct clone_args args = {
            .flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM | CLONE_SETTLS,
            .tls = threadPointer,
            .stack = threadArea,
            .stack_size = stackSize
        };
        int32_t ret = sys_clone(&args, sizeof(args), thread, (void *)-6);
        if (ret < 0) return 3;
    }

    debug_printNum("parent test1 = ", test1, "\n");
    debug_printNum("parent test2 = ", test2, "\n");
    debug_printNum("parent test3 = ", test3, "\n");
    debug_printNum("parent test4 = ", test4, "\n");
    debug_printNum("parent test5 = ", test5, "\n");
    debug_printNum("parent test6 = ", test6, "\n");
    debug_printNum("parent &test1 = ", (int64_t)&test1, "\n");
    debug_printNum("parent &test2 = ", (int64_t)&test2, "\n");
    debug_printNum("parent &test3 = ", (int64_t)&test3, "\n");
    debug_printNum("parent &test4 = ", (int64_t)&test4, "\n");
    debug_printNum("parent &test5 = ", (int64_t)&test5, "\n");
    debug_printNum("parent &test6 = ", (int64_t)&test6, "\n");

    // Wait for child to finish.
    for (;;) {
        if (hc_ATOMIC_LOAD(&childDone, hc_ATOMIC_ACQUIRE) == CHILD_DONE) break;
        int32_t hc_UNUSED status = sys_futex(&childDone, FUTEX_WAIT_PRIVATE, CHILD_NOT_DONE, NULL, NULL, 0);
        debug_ASSERT(status == 0 || status == -EAGAIN);
    }

    return 0;
}
