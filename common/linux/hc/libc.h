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

#define INT8_MIN (-1 - 0x7f)
#define INT16_MIN (-1 - 0x7fff)
#define INT32_MIN (-1 - 0x7fffffff)
#define INT64_MIN (-1 - 0x7fffffffffffffff)
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
#if hc_X86_64
hc_PACKED
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

void *memset(void *dest, int32_t c, uint64_t n);
void *memmove(void *dest, const void *src, uint64_t n);
void *memcpy(void *restrict dest, const void *restrict src, uint64_t n);
int32_t memcmp(const void *left, const void *right, uint64_t n);