#include "../../hc/hc.h"
#include "../../hc/libc.h"
#include "../../hc/libc/libc.c"
#include "../../hc/wrappers.c"
#include "../../hc/libhc/debug.c"

// 1 to use spin lock, 0 for futex-based locking.
#define SPIN 0

#define UNLOCKED 0
#define LOCKED 1

#define CHILD_NOT_DONE 0
#define CHILD_DONE 1


static char stack[4096] hc_ALIGNED(16);

static int32_t printLock = UNLOCKED;
static int32_t childDone = CHILD_NOT_DONE;

static void takeLock(void) {
    for (;;) {
        if (hc_ATOMIC_EXCHANGE(&printLock, LOCKED, hc_ATOMIC_ACQUIRE) == UNLOCKED) break;

#if SPIN
        while (hc_ATOMIC_LOAD(&printLock, hc_ATOMIC_RELAXED) == LOCKED) hc_ATOMIC_PAUSE;
#else
        int32_t hc_UNUSED status = hc_futex(&printLock, FUTEX_WAIT_PRIVATE, LOCKED, NULL, NULL, 0);
        debug_ASSERT(status == 0 || status == -EAGAIN);
#endif
    }
}

static void releaseLock(void) {
    hc_ATOMIC_STORE(&printLock, UNLOCKED, hc_ATOMIC_RELEASE);

#if !SPIN
    int32_t hc_UNUSED status = hc_futex(&printLock, FUTEX_WAKE_PRIVATE, 1, NULL, NULL, 0);
    debug_ASSERT(status >= 0);
#endif
}

static noreturn void thread(void *arg) {
    for (int32_t i = 0; i < 1000000; ++i) {
        takeLock();
        debug_printNum((char *)arg, i, "\n");
        releaseLock();
    }

    hc_ATOMIC_STORE(&childDone, CHILD_DONE, hc_ATOMIC_RELEASE);
    int32_t hc_UNUSED status = hc_futex(&childDone, FUTEX_WAKE_PRIVATE, 1, NULL, NULL, 0);
    debug_ASSERT(status >= 0);

    hc_exit(0);
}

int32_t main(hc_UNUSED int32_t argc, hc_UNUSED char **argv) {
    // Start child thread.
    struct clone_args args = {
        .flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM,
        .stack = &stack[0],
        .stack_size = sizeof(stack)
    };
    int32_t ret = hc_clone(&args, sizeof(args), thread, (void *)"child");
    if (ret < 0) return 1;

    for (int32_t i = 0; i < 1000000; ++i) {
        takeLock();
        debug_printNum("parent", i, "\n");
        releaseLock();
    }

    // Wait for child to finish.
    for (;;) {
        if (hc_ATOMIC_LOAD(&childDone, hc_ATOMIC_ACQUIRE) == CHILD_DONE) break;
        int32_t hc_UNUSED status = hc_futex(&childDone, FUTEX_WAIT_PRIVATE, CHILD_NOT_DONE, NULL, NULL, 0);
        debug_ASSERT(status == 0 || status == -EAGAIN);
    }

    return 0;
}