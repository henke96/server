// Syscall wrappers
static inline int64_t hc_write(int32_t fd, const void *buf, int64_t count) {
    hc_SYSCALL3(hc_NR_write, fd, buf, count);
    return ret;
}

static inline int32_t hc_close(int32_t fd) {
    hc_SYSCALL1(hc_NR_close, fd);
    return (int32_t)ret;
}

static inline int64_t hc_read(int32_t fd, void *buf, int64_t count) {
    hc_SYSCALL3(hc_NR_read, fd, buf, count);
    return ret;
}

static inline _Noreturn void hc_exit_group(int32_t exitcode) {
    hc_SYSCALL1(hc_NR_exit_group, exitcode);
    hc_UNREACHABLE;
}

static inline int32_t hc_getpid(void) {
    hc_SYSCALL0(hc_NR_getpid);
    return (int32_t)ret;
}

static inline int32_t hc_kill(int32_t pid, int32_t signal) {
    hc_SYSCALL2(hc_NR_kill, pid, signal);
    return (int32_t)ret;
}

static inline int32_t hc_socket(int32_t family, int32_t type, int32_t protocol) {
    hc_SYSCALL3(hc_NR_socket, family, type, protocol);
    return (int32_t)ret;
}

static inline int32_t hc_setsockopt(int32_t fd, int32_t level, int32_t optname, const void *optval, int32_t optlen) {
    hc_SYSCALL5(hc_NR_setsockopt, fd, level, optname, optval, optlen);
    return (int32_t)ret;
}

static inline int32_t hc_bind(int32_t fd, const void *addr, int32_t addrlen) {
    hc_SYSCALL3(hc_NR_bind, fd, addr, addrlen);
    return (int32_t)ret;
}

static inline int32_t hc_listen(int32_t fd, int32_t backlog) {
    hc_SYSCALL2(hc_NR_listen, fd, backlog);
    return (int32_t)ret;
}

static inline int32_t hc_accept4(int32_t fd, void *addr, int32_t *restrict addrlen, uint32_t flags) {
    hc_SYSCALL4(hc_NR_accept4, fd, addr, addrlen, flags);
    return (int32_t)ret;
}

static inline int64_t hc_sendto(int32_t fd, const void *buf, int64_t len, uint32_t flags, const void *addr, int32_t addrlen) {
    hc_SYSCALL6(hc_NR_sendto, fd, buf, len, flags, addr, addrlen);
    return ret;
}

static inline int64_t hc_sendmsg(int32_t fd, const struct msghdr *msg, uint32_t flags) {
    hc_SYSCALL3(hc_NR_sendmsg, fd, msg, flags);
    return ret;
}

static inline int64_t hc_recvfrom(int32_t fd, const void *restrict buf, int64_t len, uint32_t flags, const void *restrict addr, int32_t *restrict addrlen) {
    hc_SYSCALL6(hc_NR_recvfrom, fd, buf, len, flags, addr, addrlen);
    return ret;
}

static inline int32_t hc_epoll_create1(uint32_t flags) {
    hc_SYSCALL1(hc_NR_epoll_create1, flags);
    return (int32_t)ret;
}

static inline int32_t hc_epoll_ctl(int32_t epfd, int32_t op, int32_t fd, struct epoll_event *event) {
    hc_SYSCALL4(hc_NR_epoll_ctl, epfd, op, fd, event);
    return (int32_t)ret;
}

static inline int32_t hc_epoll_pwait(int32_t epfd, struct epoll_event *events, int32_t maxevents, int32_t timeout, const int64_t *sigmask) {
    hc_SYSCALL6(hc_NR_epoll_pwait, epfd, events, maxevents, timeout, sigmask, sizeof(*sigmask));
    return (int32_t)ret;
}

static inline int32_t hc_timerfd_create(int32_t clockid, uint32_t flags) {
    hc_SYSCALL2(hc_NR_timerfd_create, clockid, flags);
    return (int32_t)ret;
}

static inline int32_t hc_timerfd_settime(int32_t fd, uint32_t flags, const struct itimerspec *new, struct itimerspec *old) {
    hc_SYSCALL4(hc_NR_timerfd_settime, fd, flags, new, old);
    return (int32_t)ret;
}

static inline void *hc_mmap(void *addr, int64_t length, int32_t prot, uint32_t flags, int32_t fd, int64_t offset) {
    hc_SYSCALL6(hc_NR_mmap, addr, length, prot, flags, fd, offset);
    return (void *)ret;
}

static inline int32_t hc_munmap(void *addr, int64_t length) {
    hc_SYSCALL2(hc_NR_munmap, addr, length);
    return (int32_t)ret;
}

static inline void *hc_mremap(void *oldaddr, int64_t oldsize, int64_t newsize, uint32_t flags) {
    hc_SYSCALL5(hc_NR_mremap, oldaddr, oldsize, newsize, flags, NULL);
    return (void *)ret;
}

static inline int32_t hc_clock_gettime(int32_t clock, struct timespec *time) {
    hc_SYSCALL2(hc_NR_clock_gettime, clock, time);
    return (int32_t)ret;
}

static inline int64_t hc_getrandom(void *buf, int64_t buflen, uint32_t flags) {
    hc_SYSCALL3(hc_NR_getrandom, buf, buflen, flags);
    return ret;
}
