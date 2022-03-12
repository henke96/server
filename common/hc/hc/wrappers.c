// Syscall wrappers
hc_UNUSED
static hc_ALWAYS_INLINE int64_t hc_write(int32_t fd, const void *buf, int64_t count) {
    hc_SYSCALL3(hc_NR_write, fd, buf, count);
    return ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int64_t hc_writev(int32_t fd, const struct iovec *iov, int32_t iovlen) {
    hc_SYSCALL3(hc_NR_writev, fd, iov, iovlen);
    return ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_close(int32_t fd) {
    hc_SYSCALL1(hc_NR_close, fd);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_openat(int32_t dirfd, const void *pathname, uint32_t flags, uint16_t mode) {
    hc_SYSCALL4(hc_NR_openat, dirfd, pathname, flags, mode);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int64_t hc_read(int32_t fd, void *buf, int64_t count) {
    hc_SYSCALL3(hc_NR_read, fd, buf, count);
    return ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE _Noreturn void hc_exit(int32_t exitcode) {
    hc_SYSCALL1(hc_NR_exit, exitcode);
    hc_UNREACHABLE;
}

hc_UNUSED
static hc_ALWAYS_INLINE _Noreturn void hc_exit_group(int32_t exitcode) {
    hc_SYSCALL1(hc_NR_exit_group, exitcode);
    hc_UNREACHABLE;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_getpid(void) {
    hc_SYSCALL0(hc_NR_getpid);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_kill(int32_t pid, int32_t signal) {
    hc_SYSCALL2(hc_NR_kill, pid, signal);
    return (int32_t)ret;
}

#define hc_SIGMASK(SIGNAL) (1UL << ((SIGNAL) - 1))
hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_rt_sigprocmask(uint32_t how, uint64_t *set, uint64_t *oldset) {
    hc_SYSCALL4(hc_NR_rt_sigprocmask, how, set, oldset, 8);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_signalfd4(int32_t fd, const uint64_t *mask, uint32_t flags) {
    hc_SYSCALL4(hc_NR_signalfd4, fd, mask, 8, flags);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_socket(int32_t family, int32_t type, int32_t protocol) {
    hc_SYSCALL3(hc_NR_socket, family, type, protocol);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_setsockopt(int32_t fd, int32_t level, int32_t optname, const void *optval, int32_t optlen) {
    hc_SYSCALL5(hc_NR_setsockopt, fd, level, optname, optval, optlen);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_connect(int32_t fd, const void *addr, int32_t addrlen) {
    hc_SYSCALL3(hc_NR_connect, fd, addr, addrlen);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_bind(int32_t fd, const void *addr, int32_t addrlen) {
    hc_SYSCALL3(hc_NR_bind, fd, addr, addrlen);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_listen(int32_t fd, int32_t backlog) {
    hc_SYSCALL2(hc_NR_listen, fd, backlog);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_accept4(int32_t fd, void *addr, int32_t *restrict addrlen, uint32_t flags) {
    hc_SYSCALL4(hc_NR_accept4, fd, addr, addrlen, flags);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int64_t hc_sendto(int32_t fd, const void *buf, int64_t len, uint32_t flags, const void *addr, int32_t addrlen) {
    hc_SYSCALL6(hc_NR_sendto, fd, buf, len, flags, addr, addrlen);
    return ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int64_t hc_sendmsg(int32_t fd, const struct msghdr *msg, uint32_t flags) {
    hc_SYSCALL3(hc_NR_sendmsg, fd, msg, flags);
    return ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int64_t hc_recvfrom(int32_t fd, const void *restrict buf, int64_t len, uint32_t flags, const void *restrict addr, int32_t *restrict addrlen) {
    hc_SYSCALL6(hc_NR_recvfrom, fd, buf, len, flags, addr, addrlen);
    return ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_epoll_create1(uint32_t flags) {
    hc_SYSCALL1(hc_NR_epoll_create1, flags);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_epoll_ctl(int32_t epfd, int32_t op, int32_t fd, struct epoll_event *event) {
    hc_SYSCALL4(hc_NR_epoll_ctl, epfd, op, fd, event);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_epoll_pwait(int32_t epfd, struct epoll_event *events, int32_t maxevents, int32_t timeout, const int64_t *sigmask) {
    hc_SYSCALL6(hc_NR_epoll_pwait, epfd, events, maxevents, timeout, sigmask, sizeof(*sigmask));
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_timerfd_create(int32_t clockid, uint32_t flags) {
    hc_SYSCALL2(hc_NR_timerfd_create, clockid, flags);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_timerfd_settime(int32_t fd, uint32_t flags, const struct itimerspec *new, struct itimerspec *old) {
    hc_SYSCALL4(hc_NR_timerfd_settime, fd, flags, new, old);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE void *hc_mmap(void *addr, int64_t length, int32_t prot, uint32_t flags, int32_t fd, int64_t offset) {
    hc_SYSCALL6(hc_NR_mmap, addr, length, prot, flags, fd, offset);
    return (void *)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_munmap(void *addr, int64_t length) {
    hc_SYSCALL2(hc_NR_munmap, addr, length);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE void *hc_mremap(void *oldaddr, int64_t oldsize, int64_t newsize, uint32_t flags) {
    hc_SYSCALL5(hc_NR_mremap, oldaddr, oldsize, newsize, flags, NULL);
    return (void *)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_madvise(void *addr, int64_t size, uint32_t advice) {
    hc_SYSCALL3(hc_NR_madvise, addr, size, advice);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_clock_gettime(int32_t clock, struct timespec *time) {
    hc_SYSCALL2(hc_NR_clock_gettime, clock, time);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_clock_nanosleep(int32_t clock, uint32_t flags, const struct timespec *request, struct timespec *remain) {
    hc_SYSCALL4(hc_NR_clock_nanosleep, clock, flags, request, remain);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int64_t hc_getrandom(void *buf, int64_t buflen, uint32_t flags) {
    hc_SYSCALL3(hc_NR_getrandom, buf, buflen, flags);
    return ret;
}

// Returns pid, or negative error code.
int32_t hc_clone(struct clone_args *args, uint64_t size, void (*childfunc)(void *), void *childarg);
#if hc_X86_64
asm(
    ".section .text.hc_clone\n"
    ".local hc_clone\n"
    ".type hc_clone, @function\n"
    "hc_clone:\n"
    "mov %rcx, %r10\n"
    "mov $435, %eax\n"
    "syscall\n"
    "test %eax, %eax\n"
    "jnz 1f\n"
    "xor %ebp, %ebp\n"
    "mov %r10, %rdi\n"
    "call *%rdx\n"
    "1: ret\n"
);
#elif hc_AARCH64
asm(
    ".section .text.hc_clone\n"
    ".local hc_clone\n"
    ".type hc_clone, @function\n"
    "hc_clone:\n"
    "mov x8, #435\n"
    "svc #0\n"
    "cbz x0, 1f\n"
    "ret\n"
    "1: mov x0, x3\n"
    "blr x2\n"
);
#endif

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_clone3(struct clone_args *args, uint64_t size) {
    hc_SYSCALL2(hc_NR_clone3, args, size);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_wait4(int32_t pid, int32_t *status, int32_t options) {
    hc_SYSCALL4(hc_NR_wait4, pid, status, options, NULL);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_futex(int32_t *addr, int32_t op, int32_t val, struct timespec *timeout, int32_t *addr2, int32_t val3) {
    hc_SYSCALL6(hc_NR_futex, addr, op, val, timeout, addr2, val3);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_setsid(void) {
    hc_SYSCALL0(hc_NR_setsid);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t hc_ioctl(int32_t fd, uint32_t cmd, void *arg) {
    hc_SYSCALL3(hc_NR_ioctl, fd, cmd, arg);
    return (int32_t)ret;
}