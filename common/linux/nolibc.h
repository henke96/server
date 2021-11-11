// Static assert some compiler assumptions.
_Static_assert(sizeof(long) == 8, "long not 8 bytes");
_Static_assert(-1 == ~0, "not two's complement");
_Static_assert((-1 >> 1) == -1, "not arithmetic shift right");

#if defined(__x86_64__)
#define nolibc_X86_64 1
#elif defined(__aarch64__)
#define nolibc_AARCH64 1
#endif

// ALIGN must be power of 2.
#define nolibc_ALIGN_FORWARD(X, ALIGN) (((X) + ((ALIGN) - 1)) & ~((ALIGN) - 1))
#define nolibc_UNREACHABLE __builtin_unreachable()
#define nolibc_UNUSED(X) __attribute__((unused)) X
#define nolibc_PACKED __attribute__((packed))
#define nolibc_ABS(N) __builtin_abs((N))
#define nolibc_MEMCPY(DEST, SRC, N) __builtin_memcpy((DEST), (SRC), (N))
#define nolibc_MEMMOVE(DEST, SRC, N) __builtin_memmove((DEST), (SRC), (N))
#define nolibc_MEMCMP(LEFT, RIGHT, N) __builtin_memcmp((LEFT), (RIGHT), (N))

// C and Linux headers.
#define NULL ((void *)0)

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long uint64_t;
typedef long int64_t;

#define INT8_MIN (-1-0x7f)
#define INT16_MIN (-1-0x7fff)
#define INT32_MIN (-1-0x7fffffff)
#define INT64_MIN (-1-0x7fffffffffffffff)
#define INT8_MAX (0x7f)
#define INT16_MAX (0x7fff)
#define INT32_MAX (0x7fffffff)
#define INT64_MAX (0x7fffffffffffffff)
#define UINT8_MAX (0xff)
#define UINT16_MAX (0xffff)
#define UINT32_MAX (0xffffffffU)
#define UINT64_MAX (0xffffffffffffffffU)

#define bool _Bool
#define false 0
#define true 1

#define static_assert _Static_assert
#define noreturn _Noreturn
#define alignas _Alignas
#define alignof _Alignof

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

// errno.h
#define EPERM 1 // Operation not permitted
#define ENOENT 2 // No such file or directory
#define ESRCH 3 // No such process
#define EINTR 4 // Interrupted system call
#define EIO 5 // I/O error
#define ENXIO 6 // No such device or address
#define E2BIG 7 // Argument list too long
#define ENOEXEC 8 // Exec format error
#define EBADF 9 // Bad file number
#define ECHILD 10 // No child processes
#define EAGAIN 11 // Try again
#define ENOMEM 12 // Out of memory
#define EACCES 13 // Permission denied
#define EFAULT 14 // Bad address
#define ENOTBLK 15 // Block device required
#define EBUSY 16 // Device or resource busy
#define EEXIST 17 // File exists
#define EXDEV 18 // Cross-device link
#define ENODEV 19 // No such device
#define ENOTDIR 20 // Not a directory
#define EISDIR 21 // Is a directory
#define EINVAL 22 // Invalid argument
#define ENFILE 23 // File table overflow
#define EMFILE 24 // Too many open files
#define ENOTTY 25 // Not a typewriter
#define ETXTBSY 26 // Text file busy
#define EFBIG 27 // File too large
#define ENOSPC 28 // No space left on device
#define ESPIPE 29 // Illegal seek
#define EROFS 30 // Read-only file system
#define EMLINK 31 // Too many links
#define EPIPE 32 // Broken pipe
#define EDOM 33 // Math argument out of domain of func
#define ERANGE 34 // Math result not representable
#define EDEADLK 35 // Resource deadlock would occur
#define ENAMETOOLONG 36 // File name too long
#define ENOLCK 37 // No record locks available
#define ENOSYS 38 // Invalid system call number
#define ENOTEMPTY 39 // Directory not empty
#define ELOOP 40 // Too many symbolic links encountered
#define EWOULDBLOCK EAGAIN // Operation would block
#define ENOMSG 42 // No message of desired type
#define EIDRM 43 // Identifier removed
#define ECHRNG 44 // Channel number out of range
#define EL2NSYNC 45 // Level 2 not synchronized
#define EL3HLT 46 // Level 3 halted
#define EL3RST 47 // Level 3 reset
#define ELNRNG 48 // Link number out of range
#define EUNATCH 49 // Protocol driver not attached
#define ENOCSI 50 // No CSI structure available
#define EL2HLT 51 // Level 2 halted
#define EBADE 52 // Invalid exchange
#define EBADR 53 // Invalid request descriptor
#define EXFULL 54 // Exchange full
#define ENOANO 55 // No anode
#define EBADRQC 56 // Invalid request code
#define EBADSLT 57 // Invalid slot
#define EDEADLOCK EDEADLK
#define EBFONT 59 // Bad font file format
#define ENOSTR 60 // Device not a stream
#define ENODATA 61 // No data available
#define ETIME 62 // Timer expired
#define ENOSR 63 // Out of streams resources
#define ENONET 64 // Machine is not on the network
#define ENOPKG 65 // Package not installed
#define EREMOTE 66 // Object is remote
#define ENOLINK 67 // Link has been severed
#define EADV 68 // Advertise error
#define ESRMNT 69 // Srmount error
#define ECOMM 70 // Communication error on send
#define EPROTO 71 // Protocol error
#define EMULTIHOP 72 // Multihop attempted
#define EDOTDOT 73 // RFS specific error
#define EBADMSG 74 // Not a data message
#define EOVERFLOW 75 // Value too large for defined data type
#define ENOTUNIQ 76 // Name not unique on network
#define EBADFD 77 // File descriptor in bad state
#define EREMCHG 78 // Remote address changed
#define ELIBACC 79 // Can not access a needed shared library
#define ELIBBAD 80 // Accessing a corrupted shared library
#define ELIBSCN 81 // .lib section in a.out corrupted
#define ELIBMAX 82 // Attempting to link in too many shared libraries
#define ELIBEXEC 83 // Cannot exec a shared library directly
#define EILSEQ 84 // Illegal byte sequence
#define ERESTART 85 // Interrupted system call should be restarted
#define ESTRPIPE 86 // Streams pipe error
#define EUSERS 87 // Too many users
#define ENOTSOCK 88 // Socket operation on non-socket
#define EDESTADDRREQ 89 // Destination address required
#define EMSGSIZE 90 // Message too long
#define EPROTOTYPE 91 // Protocol wrong type for socket
#define ENOPROTOOPT 92 // Protocol not available
#define EPROTONOSUPPORT 93 // Protocol not supported
#define ESOCKTNOSUPPORT 94 // Socket type not supported
#define EOPNOTSUPP 95 // Operation not supported on transport endpoint
#define EPFNOSUPPORT 96 // Protocol family not supported
#define EAFNOSUPPORT 97 // Address family not supported by protocol
#define EADDRINUSE 98 // Address already in use
#define EADDRNOTAVAIL 99 // Cannot assign requested address
#define ENETDOWN 100 // Network is down
#define ENETUNREACH 101 // Network is unreachable
#define ENETRESET 102 // Network dropped connection because of reset
#define ECONNABORTED 103 // Software caused connection abort
#define ECONNRESET 104 // Connection reset by peer
#define ENOBUFS 105 // No buffer space available
#define EISCONN 106 // Transport endpoint is already connected
#define ENOTCONN 107 // Transport endpoint is not connected
#define ESHUTDOWN 108 // Cannot send after transport endpoint shutdown
#define ETOOMANYREFS 109 // Too many references: cannot splice
#define ETIMEDOUT 110 // Connection timed out
#define ECONNREFUSED 111 // Connection refused
#define EHOSTDOWN 112 // Host is down
#define EHOSTUNREACH 113 // No route to host
#define EALREADY 114 // Operation already in progress
#define EINPROGRESS 115 // Operation now in progress
#define ESTALE 116 // Stale file handle
#define EUCLEAN 117 // Structure needs cleaning
#define ENOTNAM 118 // Not a XENIX named type file
#define ENAVAIL 119 // No XENIX semaphores available
#define EISNAM 120 // Is a named type file
#define EREMOTEIO 121 // Remote I/O error
#define EDQUOT 122 // Quota exceeded
#define ENOMEDIUM 123 // No medium found
#define EMEDIUMTYPE 124 // Wrong medium type
#define ECANCELED 125 // Operation Canceled
#define ENOKEY 126 // Required key not available
#define EKEYEXPIRED 127 // Key has expired
#define EKEYREVOKED 128 // Key has been revoked
#define EKEYREJECTED 129 // Key was rejected by service
#define EOWNERDEAD 130 // Owner died
#define ENOTRECOVERABLE 131 // State not recoverable
#define ERFKILL 132 // Operation not possible due to RF-kill
#define EHWPOISON 133 // Memory page has hardware error

// socket.h
#define AF_UNSPEC 0
#define AF_UNIX 1 // Unix domain sockets
#define AF_INET 2 // Internet IP Protocol
#define AF_BRIDGE 7 // Multiprotocol bridge
#define AF_INET6 10 // IP version 6
#define AF_NETLINK 16
#define AF_PACKET 17// Packet family
#define AF_ALG 38 // Algorithm sockets

#define SOCK_STREAM 1
#define SOCK_DGRAM 2
#define SOCK_RAW 3
#define SOCK_RDM 4
#define SOCK_SEQPACKET 5
#define SOCK_DCCP 6
#define SOCK_PACKET 10

#define SOL_SOCKET 1

#define SO_DEBUG 1
#define SO_REUSEADDR 2
#define SO_TYPE 3
#define SO_ERROR 4
#define SO_DONTROUTE 5
#define SO_BROADCAST 6
#define SO_SNDBUF 7
#define SO_RCVBUF 8
#define SO_SNDBUFFORCE 32
#define SO_RCVBUFFORCE 33
#define SO_KEEPALIVE 9
#define SO_OOBINLINE 10
#define SO_NO_CHECK 11
#define SO_PRIORITY 12
#define SO_LINGER 13
#define SO_BSDCOMPAT 14
#define SO_REUSEPORT 15

#define SOCK_CLOEXEC O_CLOEXEC
#define SOCK_NONBLOCK O_NONBLOCK

#define MSG_OOB 1
#define MSG_PEEK 2
#define MSG_DONTROUTE 4
#define MSG_CTRUNC 8
#define MSG_PROBE 0x10 // Do not send. Only probe path f.e. for MTU
#define MSG_TRUNC 0x20
#define MSG_DONTWAIT 0x40 // Nonblocking io
#define MSG_EOR 0x80 // End of record
#define MSG_WAITALL 0x100 // Wait for a full request
#define MSG_FIN 0x200
#define MSG_SYN 0x400
#define MSG_CONFIRM 0x800 // Confirm path validity
#define MSG_RST 0x1000
#define MSG_ERRQUEUE 0x2000 // Fetch message from error queue
#define MSG_NOSIGNAL 0x4000 // Do not generate SIGPIPE
#define MSG_MORE 0x8000 // Sender will send more
#define MSG_WAITFORONE 0x10000 // recvmmsg(): block until 1+ packets avail
#define MSG_BATCH 0x40000 // sendmmsg(): more messages coming
#define MSG_SENDPAGE_DECRYPTED 0x100000
#define MSG_ZEROCOPY 0x4000000 // Use user data in kernel path
#define MSG_FASTOPEN 0x20000000 // Send data in TCP SYN

struct iovec {
    void *iov_base;
    int64_t iov_len;
};

struct msghdr {
    void *msg_name;
    int32_t msg_namelen;
    int32_t __pad1;
    struct iovec *msg_iov;
    int64_t msg_iovlen;
    void *msg_control;
    int64_t msg_controllen;
    uint32_t msg_flags;
    int32_t __pad2;
};

// in.h
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17
#define IPPROTO_IPV6 41
#define IPPROTO_RAW 255

#define INADDR_ANY ((uint32_t)0x00000000)
#define INADDR_BROADCAST ((uint32_t)0xffffffff)
#define INADDR_NONE ((uint32_t)0xffffffff)

struct in_addr {
    uint32_t s_addr;
};

struct sockaddr_in {
    uint16_t sin_family;
    uint16_t sin_port;
    struct in_addr sin_addr;
    uint8_t sin_zero[8];
};

// tcp.h
#define TCP_NODELAY 1

// fcntl.h
#define O_ACCMODE 00000003
#define O_RDONLY 00000000
#define O_WRONLY 00000001
#define O_RDWR 00000002
#define O_CREAT 00000100
#define O_EXCL 00000200
#define O_NOCTTY 00000400
#define O_TRUNC 00001000
#define O_APPEND 00002000
#define O_NONBLOCK 00004000
#define O_DSYNC 00010000
#define FASYNC 00020000
#define O_DIRECT 00040000
#define O_LARGEFILE 00100000
#define O_DIRECTORY 00200000
#define O_NOFOLLOW 00400000
#define O_NOATIME 01000000
#define O_CLOEXEC 02000000

// time.h
#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1
#define CLOCK_PROCESS_CPUTIME_ID 2
#define CLOCK_THREAD_CPUTIME_ID 3
#define CLOCK_MONOTONIC_RAW 4
#define CLOCK_REALTIME_COARSE 5
#define CLOCK_MONOTONIC_COARSE 6
#define CLOCK_BOOTTIME 7
#define CLOCK_REALTIME_ALARM 8
#define CLOCK_BOOTTIME_ALARM 9

struct timespec {
    int64_t tv_sec;
    int64_t tv_nsec;
};

struct itimerspec {
    struct timespec it_interval; // Timer period
    struct timespec it_value; // Timer expiration
};

// timerfd.h
#define TFD_TIMER_ABSTIME (1 << 0)
#define TFD_TIMER_CANCEL_ON_SET (1 << 1)
#define TFD_CLOEXEC O_CLOEXEC
#define TFD_NONBLOCK O_NONBLOCK

// eventpoll.h
#define EPOLL_CLOEXEC O_CLOEXEC
#define EPOLL_NONBLOCK O_NONBLOCK

#define EPOLLIN 0x001
#define EPOLLPRI 0x002
#define EPOLLOUT 0x004
#define EPOLLRDNORM 0x040
#define EPOLLNVAL 0x020
#define EPOLLRDBAND 0x080
#define EPOLLWRNORM 0x100
#define EPOLLWRBAND 0x200
#define EPOLLMSG 0x400
#define EPOLLERR 0x008
#define EPOLLHUP 0x010
#define EPOLLRDHUP 0x2000
#define EPOLLEXCLUSIVE (1U<<28)
#define EPOLLWAKEUP (1U<<29)
#define EPOLLONESHOT (1U<<30)
#define EPOLLET (1U<<31)

#define EPOLL_CTL_ADD 1
#define EPOLL_CTL_DEL 2
#define EPOLL_CTL_MOD 3

union epoll_data {
    void *ptr;
    int32_t fd;
    uint32_t u32;
    uint64_t u64;
};

struct epoll_event {
    uint32_t events;
    int32_t __pad1;
    union epoll_data data;
}
#if nolibc_X86_64
nolibc_PACKED
#endif
;

// if_alg.h
struct sockaddr_alg {
    uint16_t salg_family;
    uint8_t salg_type[14];
    uint32_t salg_feat;
    uint32_t salg_mask;
    uint8_t salg_name[64];
};

// mman.h + mman-common.h
#define MREMAP_MAYMOVE 1
#define MREMAP_FIXED 2
#define MREMAP_DONTUNMAP 4

#define MAP_SHARED 0x01 // Share changes
#define MAP_PRIVATE 0x02 // Changes are private

#define PROT_READ 0x1 // page can be read
#define PROT_WRITE 0x2 // page can be written
#define PROT_EXEC 0x4 // page can be executed
#define PROT_SEM 0x8 // page may be used for atomic ops
#define PROT_NONE 0x0 // page can not be accessed
#define PROT_GROWSDOWN 0x01000000  // mprotect flag: extend change to start of growsdown vma
#define PROT_GROWSUP 0x02000000  // mprotect flag: extend change to end of growsup vma

#define MAP_TYPE 0x0f // Mask for type of mapping
#define MAP_FIXED 0x10 // Interpret addr exactly
#define MAP_ANONYMOUS 0x20 // don't use a file

#define MAP_POPULATE 0x008000 // populate (prefault) pagetables
#define MAP_NONBLOCK 0x010000 // do not block on IO
#define MAP_STACK 0x020000 // give out an address that is best suited for process/thread stacks
#define MAP_HUGETLB 0x040000 // create a huge page mapping
#define MAP_SYNC 0x080000 // perform synchronous page faults for the mapping
#define MAP_FIXED_NOREPLACE 0x100000 // MAP_FIXED which doesn't unmap underlying mapping

#define MAP_UNINITIALIZED 0x4000000 // For anonymous mmap, memory could be uninitialized

// signal.h
#define SIGHUP 1
#define SIGINT 2
#define SIGQUIT 3
#define SIGILL 4
#define SIGTRAP 5
#define SIGABRT 6
#define SIGIOT 6
#define SIGBUS 7
#define SIGFPE 8
#define SIGKILL 9
#define SIGUSR1 10
#define SIGSEGV 11
#define SIGUSR2 12
#define SIGPIPE 13
#define SIGALRM 14
#define SIGTERM 15
#define SIGSTKFLT 16
#define SIGCHLD 17
#define SIGCONT 18
#define SIGSTOP 19
#define SIGTSTP 20
#define SIGTTIN 21
#define SIGTTOU 22
#define SIGURG 23
#define SIGXCPU 24
#define SIGXFSZ 25
#define SIGVTALRM 26
#define SIGPROF 27
#define SIGWINCH 28
#define SIGIO 29
#define SIGPOLL SIGIO

// random.h
#define GRND_NONBLOCK 0x0001
#define GRND_RANDOM 0x0002
#define GRND_INSECURE 0x0004

// Symbols expected from a libc.
void *memset(void *dest, int32_t c, uint64_t n) {
    char *d = dest;
    for (; n != 0; --n) *d++ = (char)c;
    return dest;
}

void *memmove(void *dest, const void *src, uint64_t n) {
    char *d = dest;
    const char *s = src;
    if (d < s) {
        for (; n != 0; --n) *d++ = *s++;
    } else {
        while (n) {
            --n;
            d[n] = s[n];
        }
    }
    return dest;
}

void *memcpy(void *restrict dest, const void *restrict src, uint64_t n) {
    char *d = dest;
    const char *s = src;
    for (; n != 0; --n) *d++ = *s++;
    return dest;
}

int32_t memcmp(const void *left, const void *right, uint64_t n) {
    const char *l = left;
    const char *r = right;
    int32_t diff = 0;
    for (; n != 0; --n) {
        diff = *l - *r;
        if (diff != 0) break;
        ++l;
        ++r;
    }
    return diff;
}

// Entry point
#if nolibc_X86_64
asm(
    ".section .text\n"
    ".global _start\n"
    "_start:\n"
    "pop %rdi\n"                // argc   (first arg, %rdi)
    "mov %rsp, %rsi\n"          // argv[] (second arg, %rsi)
    "lea 8(%rsi,%rdi,8),%rdx\n" // then a NULL then envp (third arg, %rdx)
    "xor %ebp, %ebp\n"          // the deepest stack frame should be zero
    "and $-16, %rsp\n"          // x86 ABI: esp must be 16-byte aligned
    "call main\n"               // main() returns the status code, we'll exit with it.
    "mov %eax, %edi\n"          // retrieve exit code
    "mov $231, %eax\n"          // NR_exit_group == 231
    "syscall\n"                 // really exit
);
#elif nolibc_AARCH64
asm(
    ".section .text\n"
    ".global _start\n"
    "_start:\n"
    "ldr x0, [sp]\n"              // argc (x0) was in the stack
    "add x1, sp, 8\n"             // argv (x1) = sp
    "lsl x2, x0, 3\n"             // envp (x2) = 8*argc ...
    "add x2, x2, 8\n"             //           + 8 (skip null)
    "add x2, x2, x1\n"            //           + argv
    "and sp, x1, -16\n"           // sp must be 16-byte aligned in the callee
    "bl main\n"                   // main() returns the status code, we'll exit with it.
    "mov x8, 94\n"                // NR_exit_group == 94
    "svc #0\n"
);
#endif

// Syscalls
#if nolibc_X86_64
#define nolibc_SYSCALL0(NUM) \
register long ret asm("rax") = (NUM); \
asm volatile ( \
    "syscall\n" \
    : "+r"(ret) \
    : \
    : "rcx", "r11", "memory" \
);

#define nolibc_SYSCALL1(NUM, ARG1) \
register long ret asm("rax") = (NUM); \
register long arg1 asm("rdi") = (long)(ARG1); \
asm volatile ( \
    "syscall\n" \
    : "+r"(ret) \
    : "r"(arg1) \
    : "rcx", "r11", "memory" \
);

#define nolibc_SYSCALL2(NUM, ARG1, ARG2) \
register long ret asm("rax") = (NUM); \
register long arg1 asm("rdi") = (long)(ARG1); \
register long arg2 asm("rsi") = (long)(ARG2); \
asm volatile ( \
    "syscall\n" \
    : "+r"(ret) \
    : "r"(arg1), "r"(arg2) \
    : "rcx", "r11", "memory" \
);

#define nolibc_SYSCALL3(NUM, ARG1, ARG2, ARG3) \
register long ret asm("rax") = (NUM); \
register long arg1 asm("rdi") = (long)(ARG1); \
register long arg2 asm("rsi") = (long)(ARG2); \
register long arg3 asm("rdx") = (long)(ARG3); \
asm volatile ( \
    "syscall\n" \
    : "+r"(ret) \
    : "r"(arg1), "r"(arg2), "r"(arg3) \
    : "rcx", "r11", "memory" \
);

#define nolibc_SYSCALL4(NUM, ARG1, ARG2, ARG3, ARG4) \
register long ret asm("rax") = (NUM); \
register long arg1 asm("rdi") = (long)(ARG1); \
register long arg2 asm("rsi") = (long)(ARG2); \
register long arg3 asm("rdx") = (long)(ARG3); \
register long arg4 asm("r10") = (long)(ARG4); \
asm volatile ( \
    "syscall\n" \
    : "+r"(ret) \
    : "r"(arg1), "r"(arg2), "r"(arg3), "r"(arg4) \
    : "rcx", "r11", "memory" \
);

#define nolibc_SYSCALL5(NUM, ARG1, ARG2, ARG3, ARG4, ARG5) \
register long ret asm("rax") = (NUM); \
register long arg1 asm("rdi") = (long)(ARG1); \
register long arg2 asm("rsi") = (long)(ARG2); \
register long arg3 asm("rdx") = (long)(ARG3); \
register long arg4 asm("r10") = (long)(ARG4); \
register long arg5 asm("r8") = (long)(ARG5); \
asm volatile ( \
    "syscall\n" \
    : "+r"(ret) \
    : "r"(arg1), "r"(arg2), "r"(arg3), "r"(arg4), "r"(arg5) \
    : "rcx", "r11", "memory" \
);

#define nolibc_SYSCALL6(NUM, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
register long ret asm("rax") = (NUM); \
register long arg1 asm("rdi") = (long)(ARG1); \
register long arg2 asm("rsi") = (long)(ARG2); \
register long arg3 asm("rdx") = (long)(ARG3); \
register long arg4 asm("r10") = (long)(ARG4); \
register long arg5 asm("r8") = (long)(ARG5); \
register long arg6 asm("r9") = (long)(ARG6); \
asm volatile ( \
    "syscall\n" \
    : "+r"(ret) \
    : "r"(arg1), "r"(arg2), "r"(arg3), "r"(arg4), "r"(arg5), "r"(arg6) \
    : "rcx", "r11", "memory" \
);

#define nolibc_NR_read 0
#define nolibc_NR_write 1
#define nolibc_NR_open 2
#define nolibc_NR_close 3
#define nolibc_NR_stat 4
#define nolibc_NR_fstat 5
#define nolibc_NR_lstat 6
#define nolibc_NR_poll 7
#define nolibc_NR_lseek 8
#define nolibc_NR_mmap 9
#define nolibc_NR_mprotect 10
#define nolibc_NR_munmap 11
#define nolibc_NR_brk 12
#define nolibc_NR_rt_sigaction 13
#define nolibc_NR_rt_sigprocmask 14
#define nolibc_NR_rt_sigreturn 15
#define nolibc_NR_ioctl 16
#define nolibc_NR_pread64 17
#define nolibc_NR_pwrite64 18
#define nolibc_NR_readv 19
#define nolibc_NR_writev 20
#define nolibc_NR_access 21
#define nolibc_NR_pipe 22
#define nolibc_NR_select 23
#define nolibc_NR_sched_yield 24
#define nolibc_NR_mremap 25
#define nolibc_NR_msync 26
#define nolibc_NR_mincore 27
#define nolibc_NR_madvise 28
#define nolibc_NR_shmget 29
#define nolibc_NR_shmat 30
#define nolibc_NR_shmctl 31
#define nolibc_NR_dup 32
#define nolibc_NR_dup2 33
#define nolibc_NR_pause 34
#define nolibc_NR_nanosleep 35
#define nolibc_NR_getitimer 36
#define nolibc_NR_alarm 37
#define nolibc_NR_setitimer 38
#define nolibc_NR_getpid 39
#define nolibc_NR_sendfile 40
#define nolibc_NR_socket 41
#define nolibc_NR_connect 42
#define nolibc_NR_accept 43
#define nolibc_NR_sendto 44
#define nolibc_NR_recvfrom 45
#define nolibc_NR_sendmsg 46
#define nolibc_NR_recvmsg 47
#define nolibc_NR_shutdown 48
#define nolibc_NR_bind 49
#define nolibc_NR_listen 50
#define nolibc_NR_getsockname 51
#define nolibc_NR_getpeername 52
#define nolibc_NR_socketpair 53
#define nolibc_NR_setsockopt 54
#define nolibc_NR_getsockopt 55
#define nolibc_NR_clone 56
#define nolibc_NR_fork 57
#define nolibc_NR_vfork 58
#define nolibc_NR_execve 59
#define nolibc_NR_exit 60
#define nolibc_NR_wait4 61
#define nolibc_NR_kill 62
#define nolibc_NR_uname 63
#define nolibc_NR_semget 64
#define nolibc_NR_semop 65
#define nolibc_NR_semctl 66
#define nolibc_NR_shmdt 67
#define nolibc_NR_msgget 68
#define nolibc_NR_msgsnd 69
#define nolibc_NR_msgrcv 70
#define nolibc_NR_msgctl 71
#define nolibc_NR_fcntl 72
#define nolibc_NR_flock 73
#define nolibc_NR_fsync 74
#define nolibc_NR_fdatasync 75
#define nolibc_NR_truncate 76
#define nolibc_NR_ftruncate 77
#define nolibc_NR_getdents 78
#define nolibc_NR_getcwd 79
#define nolibc_NR_chdir 80
#define nolibc_NR_fchdir 81
#define nolibc_NR_rename 82
#define nolibc_NR_mkdir 83
#define nolibc_NR_rmdir 84
#define nolibc_NR_creat 85
#define nolibc_NR_link 86
#define nolibc_NR_unlink 87
#define nolibc_NR_symlink 88
#define nolibc_NR_readlink 89
#define nolibc_NR_chmod 90
#define nolibc_NR_fchmod 91
#define nolibc_NR_chown 92
#define nolibc_NR_fchown 93
#define nolibc_NR_lchown 94
#define nolibc_NR_umask 95
#define nolibc_NR_gettimeofday 96
#define nolibc_NR_getrlimit 97
#define nolibc_NR_getrusage 98
#define nolibc_NR_sysinfo 99
#define nolibc_NR_times 100
#define nolibc_NR_ptrace 101
#define nolibc_NR_getuid 102
#define nolibc_NR_syslog 103
#define nolibc_NR_getgid 104
#define nolibc_NR_setuid 105
#define nolibc_NR_setgid 106
#define nolibc_NR_geteuid 107
#define nolibc_NR_getegid 108
#define nolibc_NR_setpgid 109
#define nolibc_NR_getppid 110
#define nolibc_NR_getpgrp 111
#define nolibc_NR_setsid 112
#define nolibc_NR_setreuid 113
#define nolibc_NR_setregid 114
#define nolibc_NR_getgroups 115
#define nolibc_NR_setgroups 116
#define nolibc_NR_setresuid 117
#define nolibc_NR_getresuid 118
#define nolibc_NR_setresgid 119
#define nolibc_NR_getresgid 120
#define nolibc_NR_getpgid 121
#define nolibc_NR_setfsuid 122
#define nolibc_NR_setfsgid 123
#define nolibc_NR_getsid 124
#define nolibc_NR_capget 125
#define nolibc_NR_capset 126
#define nolibc_NR_rt_sigpending 127
#define nolibc_NR_rt_sigtimedwait 128
#define nolibc_NR_rt_sigqueueinfo 129
#define nolibc_NR_rt_sigsuspend 130
#define nolibc_NR_sigaltstack 131
#define nolibc_NR_utime 132
#define nolibc_NR_mknod 133
#define nolibc_NR_uselib 134
#define nolibc_NR_personality 135
#define nolibc_NR_ustat 136
#define nolibc_NR_statfs 137
#define nolibc_NR_fstatfs 138
#define nolibc_NR_sysfs 139
#define nolibc_NR_getpriority 140
#define nolibc_NR_setpriority 141
#define nolibc_NR_sched_setparam 142
#define nolibc_NR_sched_getparam 143
#define nolibc_NR_sched_setscheduler 144
#define nolibc_NR_sched_getscheduler 145
#define nolibc_NR_sched_get_priority_max 146
#define nolibc_NR_sched_get_priority_min 147
#define nolibc_NR_sched_rr_get_interval 148
#define nolibc_NR_mlock 149
#define nolibc_NR_munlock 150
#define nolibc_NR_mlockall 151
#define nolibc_NR_munlockall 152
#define nolibc_NR_vhangup 153
#define nolibc_NR_modify_ldt 154
#define nolibc_NR_pivot_root 155
#define nolibc_NR__sysctl 156
#define nolibc_NR_prctl 157
#define nolibc_NR_arch_prctl 158
#define nolibc_NR_adjtimex 159
#define nolibc_NR_setrlimit 160
#define nolibc_NR_chroot 161
#define nolibc_NR_sync 162
#define nolibc_NR_acct 163
#define nolibc_NR_settimeofday 164
#define nolibc_NR_mount 165
#define nolibc_NR_umount2 166
#define nolibc_NR_swapon 167
#define nolibc_NR_swapoff 168
#define nolibc_NR_reboot 169
#define nolibc_NR_sethostname 170
#define nolibc_NR_setdomainname 171
#define nolibc_NR_iopl 172
#define nolibc_NR_ioperm 173
#define nolibc_NR_create_module 174
#define nolibc_NR_init_module 175
#define nolibc_NR_delete_module 176
#define nolibc_NR_get_kernel_syms 177
#define nolibc_NR_query_module 178
#define nolibc_NR_quotactl 179
#define nolibc_NR_nfsservctl 180
#define nolibc_NR_getpmsg 181
#define nolibc_NR_putpmsg 182
#define nolibc_NR_afs_syscall 183
#define nolibc_NR_tuxcall 184
#define nolibc_NR_security 185
#define nolibc_NR_gettid 186
#define nolibc_NR_readahead 187
#define nolibc_NR_setxattr 188
#define nolibc_NR_lsetxattr 189
#define nolibc_NR_fsetxattr 190
#define nolibc_NR_getxattr 191
#define nolibc_NR_lgetxattr 192
#define nolibc_NR_fgetxattr 193
#define nolibc_NR_listxattr 194
#define nolibc_NR_llistxattr 195
#define nolibc_NR_flistxattr 196
#define nolibc_NR_removexattr 197
#define nolibc_NR_lremovexattr 198
#define nolibc_NR_fremovexattr 199
#define nolibc_NR_tkill 200
#define nolibc_NR_time 201
#define nolibc_NR_futex 202
#define nolibc_NR_sched_setaffinity 203
#define nolibc_NR_sched_getaffinity 204
#define nolibc_NR_set_thread_area 205
#define nolibc_NR_io_setup 206
#define nolibc_NR_io_destroy 207
#define nolibc_NR_io_getevents 208
#define nolibc_NR_io_submit 209
#define nolibc_NR_io_cancel 210
#define nolibc_NR_get_thread_area 211
#define nolibc_NR_lookup_dcookie 212
#define nolibc_NR_epoll_create 213
#define nolibc_NR_epoll_ctl_old 214
#define nolibc_NR_epoll_wait_old 215
#define nolibc_NR_remap_file_pages 216
#define nolibc_NR_getdents64 217
#define nolibc_NR_set_tid_address 218
#define nolibc_NR_restart_syscall 219
#define nolibc_NR_semtimedop 220
#define nolibc_NR_fadvise64 221
#define nolibc_NR_timer_create 222
#define nolibc_NR_timer_settime 223
#define nolibc_NR_timer_gettime 224
#define nolibc_NR_timer_getoverrun 225
#define nolibc_NR_timer_delete 226
#define nolibc_NR_clock_settime 227
#define nolibc_NR_clock_gettime 228
#define nolibc_NR_clock_getres 229
#define nolibc_NR_clock_nanosleep 230
#define nolibc_NR_exit_group 231
#define nolibc_NR_epoll_wait 232
#define nolibc_NR_epoll_ctl 233
#define nolibc_NR_tgkill 234
#define nolibc_NR_utimes 235
#define nolibc_NR_vserver 236
#define nolibc_NR_mbind 237
#define nolibc_NR_set_mempolicy 238
#define nolibc_NR_get_mempolicy 239
#define nolibc_NR_mq_open 240
#define nolibc_NR_mq_unlink 241
#define nolibc_NR_mq_timedsend 242
#define nolibc_NR_mq_timedreceive 243
#define nolibc_NR_mq_notify 244
#define nolibc_NR_mq_getsetattr 245
#define nolibc_NR_kexec_load 246
#define nolibc_NR_waitid 247
#define nolibc_NR_add_key 248
#define nolibc_NR_request_key 249
#define nolibc_NR_keyctl 250
#define nolibc_NR_ioprio_set 251
#define nolibc_NR_ioprio_get 252
#define nolibc_NR_inotify_init 253
#define nolibc_NR_inotify_add_watch 254
#define nolibc_NR_inotify_rm_watch 255
#define nolibc_NR_migrate_pages 256
#define nolibc_NR_openat 257
#define nolibc_NR_mkdirat 258
#define nolibc_NR_mknodat 259
#define nolibc_NR_fchownat 260
#define nolibc_NR_futimesat 261
#define nolibc_NR_newfstatat 262
#define nolibc_NR_unlinkat 263
#define nolibc_NR_renameat 264
#define nolibc_NR_linkat 265
#define nolibc_NR_symlinkat 266
#define nolibc_NR_readlinkat 267
#define nolibc_NR_fchmodat 268
#define nolibc_NR_faccessat 269
#define nolibc_NR_pselect6 270
#define nolibc_NR_ppoll 271
#define nolibc_NR_unshare 272
#define nolibc_NR_set_robust_list 273
#define nolibc_NR_get_robust_list 274
#define nolibc_NR_splice 275
#define nolibc_NR_tee 276
#define nolibc_NR_sync_file_range 277
#define nolibc_NR_vmsplice 278
#define nolibc_NR_move_pages 279
#define nolibc_NR_utimensat 280
#define nolibc_NR_epoll_pwait 281
#define nolibc_NR_signalfd 282
#define nolibc_NR_timerfd_create 283
#define nolibc_NR_eventfd 284
#define nolibc_NR_fallocate 285
#define nolibc_NR_timerfd_settime 286
#define nolibc_NR_timerfd_gettime 287
#define nolibc_NR_accept4 288
#define nolibc_NR_signalfd4 289
#define nolibc_NR_eventfd2 290
#define nolibc_NR_epoll_create1 291
#define nolibc_NR_dup3 292
#define nolibc_NR_pipe2 293
#define nolibc_NR_inotify_init1 294
#define nolibc_NR_preadv 295
#define nolibc_NR_pwritev 296
#define nolibc_NR_rt_tgsigqueueinfo 297
#define nolibc_NR_perf_event_open 298
#define nolibc_NR_recvmmsg 299
#define nolibc_NR_fanotify_init 300
#define nolibc_NR_fanotify_mark 301
#define nolibc_NR_prlimit64 302
#define nolibc_NR_name_to_handle_at 303
#define nolibc_NR_open_by_handle_at 304
#define nolibc_NR_clock_adjtime 305
#define nolibc_NR_syncfs 306
#define nolibc_NR_sendmmsg 307
#define nolibc_NR_setns 308
#define nolibc_NR_getcpu 309
#define nolibc_NR_process_vm_readv 310
#define nolibc_NR_process_vm_writev 311
#define nolibc_NR_kcmp 312
#define nolibc_NR_finit_module 313
#define nolibc_NR_sched_setattr 314
#define nolibc_NR_sched_getattr 315
#define nolibc_NR_renameat2 316
#define nolibc_NR_seccomp 317
#define nolibc_NR_getrandom 318
#define nolibc_NR_memfd_create 319
#define nolibc_NR_kexec_file_load 320
#define nolibc_NR_bpf 321
#define nolibc_NR_execveat 322
#define nolibc_NR_userfaultfd 323
#define nolibc_NR_membarrier 324
#define nolibc_NR_mlock2 325
#define nolibc_NR_copy_file_range 326
#define nolibc_NR_preadv2 327
#define nolibc_NR_pwritev2 328
#define nolibc_NR_pkey_mprotect 329
#define nolibc_NR_pkey_alloc 330
#define nolibc_NR_pkey_free 331
#define nolibc_NR_statx 332
#define nolibc_NR_io_pgetevents 333
#define nolibc_NR_rseq 334
#define nolibc_NR_pidfd_send_signal 424
#define nolibc_NR_io_uring_setup 425
#define nolibc_NR_io_uring_enter 426
#define nolibc_NR_io_uring_register 427
#define nolibc_NR_open_tree 428
#define nolibc_NR_move_mount 429
#define nolibc_NR_fsopen 430
#define nolibc_NR_fsconfig 431
#define nolibc_NR_fsmount 432
#define nolibc_NR_fspick 433
#define nolibc_NR_pidfd_open 434
#define nolibc_NR_clone3 435
#define nolibc_NR_close_range 436
#define nolibc_NR_openat2 437
#define nolibc_NR_pidfd_getfd 438
#define nolibc_NR_faccessat2 439
#define nolibc_NR_process_madvise 440
#define nolibc_NR_epoll_pwait2 441
#define nolibc_NR_mount_setattr 442
#define nolibc_NR_quotactl_fd 443
#define nolibc_NR_landlock_create_ruleset 444
#define nolibc_NR_landlock_add_rule 445
#define nolibc_NR_landlock_restrict_self 446
#define nolibc_NR_memfd_secret 447
#define nolibc_NR_process_mrelease 448

#elif nolibc_AARCH64
#define nolibc_SYSCALL0(NUM) \
register long num asm("x8") = (NUM); \
register long ret asm("x0"); \
asm volatile ( \
    "svc 0\n" \
    : "=r"(ret) \
    : "r"(num) \
    : "memory", "cc" \
);

#define nolibc_SYSCALL1(NUM, ARG1) \
register long num asm("x8") = (NUM); \
register long ret asm("x0") = (long)(ARG1); \
asm volatile ( \
    "svc 0\n" \
    : "+r"(ret) \
    : "r"(num) \
    : "memory", "cc" \
);

#define nolibc_SYSCALL2(NUM, ARG1, ARG2) \
register long num asm("x8") = (NUM); \
register long ret asm("x0") = (long)(ARG1); \
register long arg2 asm("x1") = (long)(ARG2); \
asm volatile ( \
    "svc 0\n" \
    : "+r"(ret) \
    : "r"(arg2), "r"(num) \
    : "memory", "cc" \
);

#define nolibc_SYSCALL3(NUM, ARG1, ARG2, ARG3) \
register long num asm("x8") = (NUM); \
register long ret asm("x0") = (long)(ARG1); \
register long arg2 asm("x1") = (long)(ARG2); \
register long arg3 asm("x2") = (long)(ARG3); \
asm volatile ( \
    "svc 0\n" \
    : "+r"(ret) \
    : "r"(arg2), "r"(arg3), "r"(num) \
    : "memory", "cc" \
);

#define nolibc_SYSCALL4(NUM, ARG1, ARG2, ARG3, ARG4) \
register long num asm("x8") = (NUM); \
register long ret asm("x0") = (long)(ARG1); \
register long arg2 asm("x1") = (long)(ARG2); \
register long arg3 asm("x2") = (long)(ARG3); \
register long arg4 asm("x3") = (long)(ARG4); \
asm volatile ( \
    "svc 0\n" \
    : "+r"(ret) \
    : "r"(arg2), "r"(arg3), "r"(arg4), "r"(num) \
    : "memory", "cc" \
);

#define nolibc_SYSCALL5(NUM, ARG1, ARG2, ARG3, ARG4, ARG5) \
register long num asm("x8") = (NUM); \
register long ret asm("x0") = (long)(ARG1); \
register long arg2 asm("x1") = (long)(ARG2); \
register long arg3 asm("x2") = (long)(ARG3); \
register long arg4 asm("x3") = (long)(ARG4); \
register long arg5 asm("x4") = (long)(ARG5); \
asm volatile ( \
    "svc 0\n" \
    : "+r"(ret) \
    : "r"(arg2), "r"(arg3), "r"(arg4), "r"(arg5), "r"(num) \
    : "memory", "cc" \
);

#define nolibc_SYSCALL6(NUM, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
register long num asm("x8") = (NUM); \
register long ret asm("x0") = (long)(ARG1); \
register long arg2 asm("x1") = (long)(ARG2); \
register long arg3 asm("x2") = (long)(ARG3); \
register long arg4 asm("x3") = (long)(ARG4); \
register long arg5 asm("x4") = (long)(ARG5); \
register long arg6 asm("x5") = (long)(ARG6); \
asm volatile ( \
    "svc 0\n" \
    : "+r"(ret) \
    : "r"(arg2), "r"(arg3), "r"(arg4), "r"(arg5), "r"(arg6), "r"(num) \
    : "memory", "cc" \
);

#define nolibc_NR_io_setup 0
#define nolibc_NR_io_destroy 1
#define nolibc_NR_io_submit 2
#define nolibc_NR_io_cancel 3
#define nolibc_NR_io_getevents 4
#define nolibc_NR_setxattr 5
#define nolibc_NR_lsetxattr 6
#define nolibc_NR_fsetxattr 7
#define nolibc_NR_getxattr 8
#define nolibc_NR_lgetxattr 9
#define nolibc_NR_fgetxattr 10
#define nolibc_NR_listxattr 11
#define nolibc_NR_llistxattr 12
#define nolibc_NR_flistxattr 13
#define nolibc_NR_removexattr 14
#define nolibc_NR_lremovexattr 15
#define nolibc_NR_fremovexattr 16
#define nolibc_NR_getcwd 17
#define nolibc_NR_lookup_dcookie 18
#define nolibc_NR_eventfd2 19
#define nolibc_NR_epoll_create1 20
#define nolibc_NR_epoll_ctl 21
#define nolibc_NR_epoll_pwait 22
#define nolibc_NR_dup 23
#define nolibc_NR_dup3 24
#define nolibc_NR_fcntl 25
#define nolibc_NR_inotify_init1 26
#define nolibc_NR_inotify_add_watch 27
#define nolibc_NR_inotify_rm_watch 28
#define nolibc_NR_ioctl 29
#define nolibc_NR_ioprio_set 30
#define nolibc_NR_ioprio_get 31
#define nolibc_NR_flock 32
#define nolibc_NR_mknodat 33
#define nolibc_NR_mkdirat 34
#define nolibc_NR_unlinkat 35
#define nolibc_NR_symlinkat 36
#define nolibc_NR_linkat 37
#define nolibc_NR_renameat 38
#define nolibc_NR_umount2 39
#define nolibc_NR_mount 40
#define nolibc_NR_pivot_root 41
#define nolibc_NR_nfsservctl 42
#define nolibc_NR_statfs 43
#define nolibc_NR_fstatfs 44
#define nolibc_NR_truncate 45
#define nolibc_NR_ftruncate 46
#define nolibc_NR_fallocate 47
#define nolibc_NR_faccessat 48
#define nolibc_NR_chdir 49
#define nolibc_NR_fchdir 50
#define nolibc_NR_chroot 51
#define nolibc_NR_fchmod 52
#define nolibc_NR_fchmodat 53
#define nolibc_NR_fchownat 54
#define nolibc_NR_fchown 55
#define nolibc_NR_openat 56
#define nolibc_NR_close 57
#define nolibc_NR_vhangup 58
#define nolibc_NR_pipe2 59
#define nolibc_NR_quotactl 60
#define nolibc_NR_getdents64 61
#define nolibc_NR_lseek 62
#define nolibc_NR_read 63
#define nolibc_NR_write 64
#define nolibc_NR_readv 65
#define nolibc_NR_writev 66
#define nolibc_NR_pread64 67
#define nolibc_NR_pwrite64 68
#define nolibc_NR_preadv 69
#define nolibc_NR_pwritev 70
#define nolibc_NR_sendfile 71
#define nolibc_NR_pselect6 72
#define nolibc_NR_ppoll 73
#define nolibc_NR_signalfd4 74
#define nolibc_NR_vmsplice 75
#define nolibc_NR_splice 76
#define nolibc_NR_tee 77
#define nolibc_NR_readlinkat 78
#define nolibc_NR_newfstatat 79
#define nolibc_NR_fstat 80
#define nolibc_NR_sync 81
#define nolibc_NR_fsync 82
#define nolibc_NR_fdatasync 83
#define nolibc_NR_sync_file_range 84
#define nolibc_NR_timerfd_create 85
#define nolibc_NR_timerfd_settime 86
#define nolibc_NR_timerfd_gettime 87
#define nolibc_NR_utimensat 88
#define nolibc_NR_acct 89
#define nolibc_NR_capget 90
#define nolibc_NR_capset 91
#define nolibc_NR_personality 92
#define nolibc_NR_exit 93
#define nolibc_NR_exit_group 94
#define nolibc_NR_waitid 95
#define nolibc_NR_set_tid_address 96
#define nolibc_NR_unshare 97
#define nolibc_NR_futex 98
#define nolibc_NR_set_robust_list 99
#define nolibc_NR_get_robust_list 100
#define nolibc_NR_nanosleep 101
#define nolibc_NR_getitimer 102
#define nolibc_NR_setitimer 103
#define nolibc_NR_kexec_load 104
#define nolibc_NR_init_module 105
#define nolibc_NR_delete_module 106
#define nolibc_NR_timer_create 107
#define nolibc_NR_timer_gettime 108
#define nolibc_NR_timer_getoverrun 109
#define nolibc_NR_timer_settime 110
#define nolibc_NR_timer_delete 111
#define nolibc_NR_clock_settime 112
#define nolibc_NR_clock_gettime 113
#define nolibc_NR_clock_getres 114
#define nolibc_NR_clock_nanosleep 115
#define nolibc_NR_syslog 116
#define nolibc_NR_ptrace 117
#define nolibc_NR_sched_setparam 118
#define nolibc_NR_sched_setscheduler 119
#define nolibc_NR_sched_getscheduler 120
#define nolibc_NR_sched_getparam 121
#define nolibc_NR_sched_setaffinity 122
#define nolibc_NR_sched_getaffinity 123
#define nolibc_NR_sched_yield 124
#define nolibc_NR_sched_get_priority_max 125
#define nolibc_NR_sched_get_priority_min 126
#define nolibc_NR_sched_rr_get_interval 127
#define nolibc_NR_restart_syscall 128
#define nolibc_NR_kill 129
#define nolibc_NR_tkill 130
#define nolibc_NR_tgkill 131
#define nolibc_NR_sigaltstack 132
#define nolibc_NR_rt_sigsuspend 133
#define nolibc_NR_rt_sigaction 134
#define nolibc_NR_rt_sigprocmask 135
#define nolibc_NR_rt_sigpending 136
#define nolibc_NR_rt_sigtimedwait 137
#define nolibc_NR_rt_sigqueueinfo 138
#define nolibc_NR_rt_sigreturn 139
#define nolibc_NR_setpriority 140
#define nolibc_NR_getpriority 141
#define nolibc_NR_reboot 142
#define nolibc_NR_setregid 143
#define nolibc_NR_setgid 144
#define nolibc_NR_setreuid 145
#define nolibc_NR_setuid 146
#define nolibc_NR_setresuid 147
#define nolibc_NR_getresuid 148
#define nolibc_NR_setresgid 149
#define nolibc_NR_getresgid 150
#define nolibc_NR_setfsuid 151
#define nolibc_NR_setfsgid 152
#define nolibc_NR_times 153
#define nolibc_NR_setpgid 154
#define nolibc_NR_getpgid 155
#define nolibc_NR_getsid 156
#define nolibc_NR_setsid 157
#define nolibc_NR_getgroups 158
#define nolibc_NR_setgroups 159
#define nolibc_NR_uname 160
#define nolibc_NR_sethostname 161
#define nolibc_NR_setdomainname 162
#define nolibc_NR_getrlimit 163
#define nolibc_NR_setrlimit 164
#define nolibc_NR_getrusage 165
#define nolibc_NR_umask 166
#define nolibc_NR_prctl 167
#define nolibc_NR_getcpu 168
#define nolibc_NR_gettimeofday 169
#define nolibc_NR_settimeofday 170
#define nolibc_NR_adjtimex 171
#define nolibc_NR_getpid 172
#define nolibc_NR_getppid 173
#define nolibc_NR_getuid 174
#define nolibc_NR_geteuid 175
#define nolibc_NR_getgid 176
#define nolibc_NR_getegid 177
#define nolibc_NR_gettid 178
#define nolibc_NR_sysinfo 179
#define nolibc_NR_mq_open 180
#define nolibc_NR_mq_unlink 181
#define nolibc_NR_mq_timedsend 182
#define nolibc_NR_mq_timedreceive 183
#define nolibc_NR_mq_notify 184
#define nolibc_NR_mq_getsetattr 185
#define nolibc_NR_msgget 186
#define nolibc_NR_msgctl 187
#define nolibc_NR_msgrcv 188
#define nolibc_NR_msgsnd 189
#define nolibc_NR_semget 190
#define nolibc_NR_semctl 191
#define nolibc_NR_semtimedop 192
#define nolibc_NR_semop 193
#define nolibc_NR_shmget 194
#define nolibc_NR_shmctl 195
#define nolibc_NR_shmat 196
#define nolibc_NR_shmdt 197
#define nolibc_NR_socket 198
#define nolibc_NR_socketpair 199
#define nolibc_NR_bind 200
#define nolibc_NR_listen 201
#define nolibc_NR_accept 202
#define nolibc_NR_connect 203
#define nolibc_NR_getsockname 204
#define nolibc_NR_getpeername 205
#define nolibc_NR_sendto 206
#define nolibc_NR_recvfrom 207
#define nolibc_NR_setsockopt 208
#define nolibc_NR_getsockopt 209
#define nolibc_NR_shutdown 210
#define nolibc_NR_sendmsg 211
#define nolibc_NR_recvmsg 212
#define nolibc_NR_readahead 213
#define nolibc_NR_brk 214
#define nolibc_NR_munmap 215
#define nolibc_NR_mremap 216
#define nolibc_NR_add_key 217
#define nolibc_NR_request_key 218
#define nolibc_NR_keyctl 219
#define nolibc_NR_clone 220
#define nolibc_NR_execve 221
#define nolibc_NR_mmap 222
#define nolibc_NR_fadvise64 223
#define nolibc_NR_swapon 224
#define nolibc_NR_swapoff 225
#define nolibc_NR_mprotect 226
#define nolibc_NR_msync 227
#define nolibc_NR_mlock 228
#define nolibc_NR_munlock 229
#define nolibc_NR_mlockall 230
#define nolibc_NR_munlockall 231
#define nolibc_NR_mincore 232
#define nolibc_NR_madvise 233
#define nolibc_NR_remap_file_pages 234
#define nolibc_NR_mbind 235
#define nolibc_NR_get_mempolicy 236
#define nolibc_NR_set_mempolicy 237
#define nolibc_NR_migrate_pages 238
#define nolibc_NR_move_pages 239
#define nolibc_NR_rt_tgsigqueueinfo 240
#define nolibc_NR_perf_event_open 241
#define nolibc_NR_accept4 242
#define nolibc_NR_recvmmsg 243
#define nolibc_NR_wait4 260
#define nolibc_NR_prlimit64 261
#define nolibc_NR_fanotify_init 262
#define nolibc_NR_fanotify_mark 263
#define nolibc_NR_name_to_handle_at 264
#define nolibc_NR_open_by_handle_at 265
#define nolibc_NR_clock_adjtime 266
#define nolibc_NR_syncfs 267
#define nolibc_NR_setns 268
#define nolibc_NR_sendmmsg 269
#define nolibc_NR_process_vm_readv 270
#define nolibc_NR_process_vm_writev 271
#define nolibc_NR_kcmp 272
#define nolibc_NR_finit_module 273
#define nolibc_NR_sched_setattr 274
#define nolibc_NR_sched_getattr 275
#define nolibc_NR_renameat2 276
#define nolibc_NR_seccomp 277
#define nolibc_NR_getrandom 278
#define nolibc_NR_memfd_create 279
#define nolibc_NR_bpf 280
#define nolibc_NR_execveat 281
#define nolibc_NR_userfaultfd 282
#define nolibc_NR_membarrier 283
#define nolibc_NR_mlock2 284
#define nolibc_NR_copy_file_range 285
#define nolibc_NR_preadv2 286
#define nolibc_NR_pwritev2 287
#define nolibc_NR_pkey_mprotect 288
#define nolibc_NR_pkey_alloc 289
#define nolibc_NR_pkey_free 290
#define nolibc_NR_statx 291
#define nolibc_NR_io_pgetevents 292
#define nolibc_NR_rseq 293
#define nolibc_NR_kexec_file_load 294
#define nolibc_NR_pidfd_send_signal 424
#define nolibc_NR_io_uring_setup 425
#define nolibc_NR_io_uring_enter 426
#define nolibc_NR_io_uring_register 427
#define nolibc_NR_open_tree 428
#define nolibc_NR_move_mount 429
#define nolibc_NR_fsopen 430
#define nolibc_NR_fsconfig 431
#define nolibc_NR_fsmount 432
#define nolibc_NR_fspick 433
#define nolibc_NR_pidfd_open 434
#define nolibc_NR_clone3 435
#define nolibc_NR_close_range 436
#define nolibc_NR_openat2 437
#define nolibc_NR_pidfd_getfd 438
#define nolibc_NR_faccessat2 439
#define nolibc_NR_process_madvise 440
#define nolibc_NR_epoll_pwait2 441
#define nolibc_NR_mount_setattr 442
#define nolibc_NR_quotactl_fd 443
#define nolibc_NR_landlock_create_ruleset 444
#define nolibc_NR_landlock_add_rule 445
#define nolibc_NR_landlock_restrict_self 446
#define nolibc_NR_memfd_secret 447
#define nolibc_NR_process_mrelease 448

#endif

// Syscall wrappers
static inline int64_t nolibc_write(int32_t fd, const void *buf, int64_t count) {
    nolibc_SYSCALL3(nolibc_NR_write, fd, buf, count);
    return ret;
}

static inline int32_t nolibc_close(int32_t fd) {
    nolibc_SYSCALL1(nolibc_NR_close, fd);
    return (int32_t)ret;
}

static inline int64_t nolibc_read(int32_t fd, void *buf, int64_t count) {
    nolibc_SYSCALL3(nolibc_NR_read, fd, buf, count);
    return ret;
}

static inline _Noreturn void nolibc_exit_group(int32_t exitcode) {
    nolibc_SYSCALL1(nolibc_NR_exit_group, exitcode);
    nolibc_UNREACHABLE;
}

static inline int32_t nolibc_getpid(void) {
    nolibc_SYSCALL0(nolibc_NR_getpid);
    return (int32_t)ret;
}

static inline int32_t nolibc_kill(int32_t pid, int32_t signal) {
    nolibc_SYSCALL2(nolibc_NR_kill, pid, signal);
    return (int32_t)ret;
}

static inline int32_t nolibc_socket(int32_t family, int32_t type, int32_t protocol) {
    nolibc_SYSCALL3(nolibc_NR_socket, family, type, protocol);
    return (int32_t)ret;
}

static inline int32_t nolibc_setsockopt(int32_t fd, int32_t level, int32_t optname, const void *optval, int32_t optlen) {
    nolibc_SYSCALL5(nolibc_NR_setsockopt, fd, level, optname, optval, optlen);
    return (int32_t)ret;
}

static inline int32_t nolibc_bind(int32_t fd, const void *addr, int32_t addrlen) {
    nolibc_SYSCALL3(nolibc_NR_bind, fd, addr, addrlen);
    return (int32_t)ret;
}

static inline int32_t nolibc_listen(int32_t fd, int32_t backlog) {
    nolibc_SYSCALL2(nolibc_NR_listen, fd, backlog);
    return (int32_t)ret;
}

static inline int32_t nolibc_accept4(int32_t fd, void *addr, int32_t *restrict addrlen, uint32_t flags) {
    nolibc_SYSCALL4(nolibc_NR_accept4, fd, addr, addrlen, flags);
    return (int32_t)ret;
}

static inline int64_t nolibc_sendto(int32_t fd, const void *buf, int64_t len, uint32_t flags, const void *addr, int32_t addrlen) {
    nolibc_SYSCALL6(nolibc_NR_sendto, fd, buf, len, flags, addr, addrlen);
    return ret;
}

static inline int64_t nolibc_sendmsg(int32_t fd, const struct msghdr *msg, uint32_t flags) {
    nolibc_SYSCALL3(nolibc_NR_sendmsg, fd, msg, flags);
    return ret;
}

static inline int64_t nolibc_recvfrom(int32_t fd, const void *restrict buf, int64_t len, uint32_t flags, const void *restrict addr, int32_t *restrict addrlen) {
    nolibc_SYSCALL6(nolibc_NR_recvfrom, fd, buf, len, flags, addr, addrlen);
    return ret;
}

static inline int32_t nolibc_epoll_create1(uint32_t flags) {
    nolibc_SYSCALL1(nolibc_NR_epoll_create1, flags);
    return (int32_t)ret;
}

static inline int32_t nolibc_epoll_ctl(int32_t epfd, int32_t op, int32_t fd, struct epoll_event *event) {
    nolibc_SYSCALL4(nolibc_NR_epoll_ctl, epfd, op, fd, event);
    return (int32_t)ret;
}

static inline int32_t nolibc_epoll_pwait(int32_t epfd, struct epoll_event *events, int32_t maxevents, int32_t timeout, const int64_t *sigmask) {
    nolibc_SYSCALL6(nolibc_NR_epoll_pwait, epfd, events, maxevents, timeout, sigmask, sizeof(*sigmask));
    return (int32_t)ret;
}

static inline int32_t nolibc_timerfd_create(int32_t clockid, uint32_t flags) {
    nolibc_SYSCALL2(nolibc_NR_timerfd_create, clockid, flags);
    return (int32_t)ret;
}

static inline int32_t nolibc_timerfd_settime(int32_t fd, uint32_t flags, const struct itimerspec *new, struct itimerspec *old) {
    nolibc_SYSCALL4(nolibc_NR_timerfd_settime, fd, flags, new, old);
    return (int32_t)ret;
}

static inline void *nolibc_mmap(void *addr, int64_t length, int32_t prot, uint32_t flags, int32_t fd, int64_t offset) {
    nolibc_SYSCALL6(nolibc_NR_mmap, addr, length, prot, flags, fd, offset);
    return (void *)ret;
}

static inline int32_t nolibc_munmap(void *addr, int64_t length) {
    nolibc_SYSCALL2(nolibc_NR_munmap, addr, length);
    return (int32_t)ret;
}

static inline void *nolibc_mremap(void *oldaddr, int64_t oldsize, int64_t newsize, uint32_t flags) {
    nolibc_SYSCALL5(nolibc_NR_mremap, oldaddr, oldsize, newsize, flags, NULL);
    return (void *)ret;
}

static inline int32_t nolibc_clock_gettime(int32_t clock, struct timespec *time) {
    nolibc_SYSCALL2(nolibc_NR_clock_gettime, clock, time);
    return (int32_t)ret;
}

static inline int64_t nolibc_getrandom(void *buf, int64_t buflen, uint32_t flags) {
    nolibc_SYSCALL3(nolibc_NR_getrandom, buf, buflen, flags);
    return ret;
}
