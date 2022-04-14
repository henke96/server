_Static_assert(!hc_ILP32, "Pointers not 64 bit");

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
#define EWOULDBLOCK 11 // Operation would block
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
#define EDEADLOCK 35
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

#define SOCK_CLOEXEC 02000000
#define SOCK_NONBLOCK 00004000

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
#define O_NOATIME 01000000
#define O_CLOEXEC 02000000
#if hc_AARCH64
#define O_DIRECTORY 040000 // must be a directory
#define O_NOFOLLOW 0100000 // don't follow links
#define O_DIRECT 0200000 // direct disk access hint - currently ignored
#define O_LARGEFILE 0400000
#else
#define O_DIRECTORY 00200000
#define O_NOFOLLOW 00400000
#define O_DIRECT 00040000
#define O_LARGEFILE 00100000
#endif

#define AT_FDCWD -100 // Special value used to indicate openat should use the current working directory.

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
#define TFD_CLOEXEC 02000000
#define TFD_NONBLOCK 00004000

// eventpoll.h
#define EPOLL_CLOEXEC 02000000
#define EPOLL_NONBLOCK 00004000

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
#define PROT_GROWSDOWN 0x01000000 // mprotect flag: extend change to start of growsdown vma
#define PROT_GROWSUP 0x02000000 // mprotect flag: extend change to end of growsup vma

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

#define MADV_NORMAL 0 // no further special treatment
#define MADV_RANDOM 1 // expect random page references
#define MADV_SEQUENTIAL 2 // expect sequential page references
#define MADV_WILLNEED 3 // will need these pages
#define MADV_DONTNEED 4 // don't need these pages
#define MADV_FREE 8 // free pages only if memory pressure
#define MADV_REMOVE 9 // remove these pages & resources
#define MADV_DONTFORK 10 // don't inherit across fork
#define MADV_DOFORK 11 // do inherit across fork
#define MADV_HWPOISON 100 // poison a page for testing
#define MADV_SOFT_OFFLINE 101 // soft offline page for testing
#define MADV_MERGEABLE 12 // KSM may merge identical pages
#define MADV_UNMERGEABLE 13 // KSM may not merge identical pages
#define MADV_HUGEPAGE 14 // Worth backing with hugepages
#define MADV_NOHUGEPAGE 15 // Not worth backing with hugepages
#define MADV_DONTDUMP 16 // Explicity exclude from the core dump, overrides the coredump filter bits
#define MADV_DODUMP 17 // Clear the MADV_DONTDUMP flag
#define MADV_WIPEONFORK 18 // Zero memory on fork, child only
#define MADV_KEEPONFORK 19 // Undo MADV_WIPEONFORK
#define MADV_COLD 20 // deactivate these pages
#define MADV_PAGEOUT 21 // reclaim these pages
#define MADV_POPULATE_READ 22 // populate (prefault) page tables readable
#define MADV_POPULATE_WRITE 23 // populate (prefault) page tables writable

// signal.h + siginfo.h
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

#define SA_RESTORER 0x04000000
#define SA_NOCLDSTOP 0x00000001 // flag to turn off SIGCHLD when children stop.
#define SA_NOCLDWAIT 0x00000002 // flag on SIGCHLD to inhibit zombies.
#define SA_SIGINFO 0x00000004 // delivers the signal with SIGINFO structs.
#define SA_EXPOSE_TAGBITS 0x00000800 // exposes an architecture-defined set of tag bits in siginfo.si_addr.
#define SA_ONSTACK 0x08000000 // indicates that a registered stack_t will be used.
#define SA_RESTART 0x10000000 // flag to get restarting signals (which were the default long ago)
#define SA_NODEFER 0x40000000 // prevents the current signal from being masked in the handler.
#define SA_RESETHAND 0x80000000 // clears the handler when the signal is delivered.

#define SA_NOMASK 0x40000000
#define SA_ONESHOT 0x80000000

#define SIG_BLOCK 0 // for blocking signals
#define SIG_UNBLOCK 1 // for unblocking signals
#define SIG_SETMASK 2 // for setting the signal mask

union sigval {
    int32_t sival_int;
    void *sival_ptr;
};

struct siginfo {
    int32_t si_signo;
    int32_t si_code;
    int32_t si_errno;
    int32_t __pad;
    union {
        char __pad[128 - 16];
        // kill()
        struct {
            int32_t _pid; // sender's pid
            uint32_t _uid; // sender's uid
        } _kill;

        // POSIX.1b timers
        struct {
            int32_t _tid; // timer id
            int32_t _overrun; // overrun count
            union sigval _sigval; // same as below
            int32_t _sys_private; // not to be passed to user
            int32_t __pad;
        } _timer;

        // POSIX.1b signals
        struct {
            int32_t _pid; // sender's pid
            uint32_t _uid; // sender's uid
            union sigval _sigval;
        } _rt;

        // SIGCHLD
        struct {
            int32_t _pid; // which child
            uint32_t _uid; // sender's uid
            int32_t _status; // exit code
            int32_t __pad;
            int64_t _utime;
            int64_t _stime;
        } _sigchld;

        // SIGILL, SIGFPE, SIGSEGV, SIGBUS, SIGTRAP, SIGEMT
        struct {
            void *_addr; // faulting insn/memory ref.

            union {
                // used on alpha and sparc
                int32_t _trapno; // TRAP # which caused the signal
                /*
                * used when si_code=BUS_MCEERR_AR or
                * used when si_code=BUS_MCEERR_AO
                */
                int16_t _addr_lsb; // LSB of the reported address
                // used when si_code=SEGV_BNDERR
                struct {
                    char _dummy_bnd[8];
                    void *_lower;
                    void *_upper;
                } _addr_bnd;
                // used when si_code=SEGV_PKUERR
                struct {
                    char _dummy_pkey[8];
                    uint32_t _pkey;
                } _addr_pkey;
                // used when si_code=TRAP_PERF
                struct {
                    uint64_t _data;
                    uint32_t _type;
                    int32_t __pad;
                } _perf;
            };
        } _sigfault;

        // SIGPOLL
        struct {
            int64_t _band; // POLL_IN, POLL_OUT, POLL_MSG
            int32_t _fd;
            int32_t __pad;
        } _sigpoll;

        // SIGSYS
        struct {
            void *_call_addr; // calling user insn
            int32_t _syscall; // triggering system call number
            uint32_t _arch; // AUDIT_ARCH_* of syscall
        } _sigsys;
    } _sifields;
}
#if hc_X86_64
hc_ALIGNED(8)
#endif
;

#define si_pid _sifields._kill._pid
#define si_uid _sifields._kill._uid
#define si_tid _sifields._timer._tid
#define si_overrun _sifields._timer._overrun
#define si_sys_private  _sifields._timer._sys_private
#define si_status _sifields._sigchld._status
#define si_utime _sifields._sigchld._utime
#define si_stime _sifields._sigchld._stime
#define si_value _sifields._rt._sigval
#define si_int _sifields._rt._sigval.sival_int
#define si_ptr _sifields._rt._sigval.sival_ptr
#define si_addr _sifields._sigfault._addr
#define si_trapno _sifields._sigfault._trapno
#define si_addr_lsb _sifields._sigfault._addr_lsb
#define si_lower _sifields._sigfault._addr_bnd._lower
#define si_upper _sifields._sigfault._addr_bnd._upper
#define si_pkey _sifields._sigfault._addr_pkey._pkey
#define si_perf_data _sifields._sigfault._perf._data
#define si_perf_type _sifields._sigfault._perf._type
#define si_band _sifields._sigpoll._band
#define si_fd _sifields._sigpoll._fd
#define si_call_addr _sifields._sigsys._call_addr
#define si_syscall _sifields._sigsys._syscall
#define si_arch _sifields._sigsys._arch

/*
 * si_code values
 * Digital reserves positive values for kernel-generated signals.
 */
#define SI_USER 0 // sent by kill, sigsend, raise
#define SI_KERNEL 0x80 // sent by the kernel from somewhere
#define SI_QUEUE -1 // sent by sigqueue
#define SI_TIMER -2 // sent by timer expiration
#define SI_MESGQ -3 // sent by real time mesq state change
#define SI_ASYNCIO -4 // sent by AIO completion
#define SI_SIGIO -5 // sent by queued SIGIO
#define SI_TKILL -6 // sent by tkill system call
#define SI_DETHREAD -7 // sent by execve() killing subsidiary threads
#define SI_ASYNCNL -60 // sent by glibc async name lookup completion

struct sigaction {
    union {
        void (*sa_handler)(int32_t sig);
        void (*sa_sigaction)(int32_t sig, struct siginfo *info, void *ucontext);
    };
    uint64_t sa_flags;
#if hc_X86_64
    void (*sa_restorer)(void);
#endif
    uint64_t sa_mask;
};

// random.h
#define GRND_NONBLOCK 0x0001
#define GRND_RANDOM 0x0002
#define GRND_INSECURE 0x0004

// auxvec.h
#define AT_NULL 0 // end of vector
#define AT_IGNORE 1 // entry should be ignored
#define AT_EXECFD 2 // file descriptor of program
#define AT_PHDR 3 // program headers for program
#define AT_PHENT 4 // size of program header entry
#define AT_PHNUM 5 // number of program headers
#define AT_PAGESZ 6 // system page size
#define AT_BASE 7 // base address of interpreter
#define AT_FLAGS 8 // flags
#define AT_ENTRY 9 // entry point of program
#define AT_NOTELF 10 // program is not ELF
#define AT_UID 11 // real uid
#define AT_EUID 12 // effective uid
#define AT_GID 13 // real gid
#define AT_EGID 14 // effective gid
#define AT_PLATFORM 15 // string identifying CPU for optimizations
#define AT_HWCAP 16 // arch dependent hints at CPU capabilities
#define AT_CLKTCK 17 // frequency at which times() increments
#define AT_SECURE 23 // secure mode boolean
#define AT_BASE_PLATFORM 24 // string identifying real platform, may differ from AT_PLATFORM
#define AT_RANDOM 25 // address of 16 random bytes
#define AT_HWCAP2 26 // extension of AT_HWCAP
#define AT_EXECFN 31 // filename of program
#define AT_SYSINFO_EHDR 33 // vDSO address
#define AT_MINSIGSTKSZ 51 // minimal stack size for signal delivery

// sched.h
#define CLONE_VM 0x00000100 // set if VM shared between processes
#define CLONE_FS 0x00000200 // set if fs info shared between processes
#define CLONE_FILES 0x00000400 // set if open files shared between processes
#define CLONE_SIGHAND 0x00000800 // set if signal handlers and blocked signals shared
#define CLONE_PIDFD 0x00001000 // set if a pidfd should be placed in parent
#define CLONE_PTRACE 0x00002000 // set if we want to let tracing continue on the child too
#define CLONE_VFORK 0x00004000 // set if the parent wants the child to wake it up on mm_release
#define CLONE_PARENT 0x00008000 // set if we want to have the same parent as the cloner
#define CLONE_THREAD 0x00010000 // Same thread group?
#define CLONE_NEWNS 0x00020000 // New mount namespace group
#define CLONE_SYSVSEM 0x00040000 // share system V SEM_UNDO semantics
#define CLONE_SETTLS 0x00080000 // create a new TLS for the child
#define CLONE_PARENT_SETTID 0x00100000 // set the TID in the parent
#define CLONE_CHILD_CLEARTID 0x00200000 // clear the TID in the child
#define CLONE_UNTRACED 0x00800000 // set if the tracing process can't force CLONE_PTRACE on this clone
#define CLONE_CHILD_SETTID 0x01000000 // set the TID in the child
#define CLONE_NEWCGROUP 0x02000000 // New cgroup namespace
#define CLONE_NEWUTS 0x04000000 // New utsname namespace
#define CLONE_NEWIPC 0x08000000 // New ipc namespace
#define CLONE_NEWUSER 0x10000000 // New user namespace
#define CLONE_NEWPID 0x20000000 // New pid namespace
#define CLONE_NEWNET 0x40000000 // New network namespace
#define CLONE_IO 0x80000000 // Clone io context
#define CLONE_CLEAR_SIGHAND 0x100000000ULL // Clear any signal handler and reset to SIG_DFL.
#define CLONE_INTO_CGROUP 0x200000000ULL // Clone into a specific cgroup given the right permissions.
#define CLONE_NEWTIME 0x00000080 // New time namespace

struct clone_args {
    uint64_t flags;
    int32_t *pidfd;
    int32_t *child_tid;
    int32_t *parent_tid;
    int64_t exit_signal;
    void *stack;
    int64_t stack_size;
    uint64_t tls;
    int32_t *set_tid;
    int64_t set_tid_size;
    int64_t cgroup;
};

// futex.h
#define FUTEX_WAIT 0
#define FUTEX_WAKE 1
#define FUTEX_FD 2
#define FUTEX_REQUEUE 3
#define FUTEX_CMP_REQUEUE 4
#define FUTEX_WAKE_OP 5
#define FUTEX_LOCK_PI 6
#define FUTEX_UNLOCK_PI 7
#define FUTEX_TRYLOCK_PI 8
#define FUTEX_WAIT_BITSET 9
#define FUTEX_WAKE_BITSET 10
#define FUTEX_WAIT_REQUEUE_PI 11
#define FUTEX_CMP_REQUEUE_PI 12
#define FUTEX_LOCK_PI2 13

#define FUTEX_PRIVATE_FLAG 128
#define FUTEX_CLOCK_REALTIME 256
#define FUTEX_CMD_MASK ~(FUTEX_PRIVATE_FLAG | FUTEX_CLOCK_REALTIME)

#define FUTEX_WAIT_PRIVATE (FUTEX_WAIT | FUTEX_PRIVATE_FLAG)
#define FUTEX_WAKE_PRIVATE (FUTEX_WAKE | FUTEX_PRIVATE_FLAG)
#define FUTEX_REQUEUE_PRIVATE (FUTEX_REQUEUE | FUTEX_PRIVATE_FLAG)
#define FUTEX_CMP_REQUEUE_PRIVATE (FUTEX_CMP_REQUEUE | FUTEX_PRIVATE_FLAG)
#define FUTEX_WAKE_OP_PRIVATE (FUTEX_WAKE_OP | FUTEX_PRIVATE_FLAG)
#define FUTEX_LOCK_PI_PRIVATE (FUTEX_LOCK_PI | FUTEX_PRIVATE_FLAG)
#define FUTEX_LOCK_PI2_PRIVATE (FUTEX_LOCK_PI2 | FUTEX_PRIVATE_FLAG)
#define FUTEX_UNLOCK_PI_PRIVATE (FUTEX_UNLOCK_PI | FUTEX_PRIVATE_FLAG)
#define FUTEX_TRYLOCK_PI_PRIVATE (FUTEX_TRYLOCK_PI | FUTEX_PRIVATE_FLAG)
#define FUTEX_WAIT_BITSET_PRIVATE (FUTEX_WAIT_BITSET | FUTEX_PRIVATE_FLAG)
#define FUTEX_WAKE_BITSET_PRIVATE (FUTEX_WAKE_BITSET | FUTEX_PRIVATE_FLAG)
#define FUTEX_WAIT_REQUEUE_PI_PRIVATE (FUTEX_WAIT_REQUEUE_PI | FUTEX_PRIVATE_FLAG)
#define FUTEX_CMP_REQUEUE_PI_PRIVATE (FUTEX_CMP_REQUEUE_PI | FUTEX_PRIVATE_FLAG)

// ioctl.h
#define _IOC_NRBITS 8
#define _IOC_TYPEBITS 8
#define _IOC_SIZEBITS 14
#define _IOC_DIRBITS 2

#define _IOC_NRMASK ((1 << _IOC_NRBITS)-1)
#define _IOC_TYPEMASK ((1 << _IOC_TYPEBITS)-1)
#define _IOC_SIZEMASK ((1 << _IOC_SIZEBITS)-1)
#define _IOC_DIRMASK ((1 << _IOC_DIRBITS)-1)

#define _IOC_NRSHIFT 0
#define _IOC_TYPESHIFT (_IOC_NRSHIFT + _IOC_NRBITS)
#define _IOC_SIZESHIFT (_IOC_TYPESHIFT + _IOC_TYPEBITS)
#define _IOC_DIRSHIFT (_IOC_SIZESHIFT + _IOC_SIZEBITS)

#define _IOC_NONE 0U
#define _IOC_WRITE 1U
#define _IOC_READ 2U

#define _IOC(dir,type,nr,size) (((dir) << _IOC_DIRSHIFT) | ((type) << _IOC_TYPESHIFT) | ((nr) << _IOC_NRSHIFT) | ((size) << _IOC_SIZESHIFT))
#define _IOC_TYPECHECK(t) (sizeof(t))

#define _IO(type,nr) _IOC(_IOC_NONE,(type),(nr),0)
#define _IOR(type,nr,size) _IOC(_IOC_READ,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOW(type,nr,size) _IOC(_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOWR(type,nr,size) _IOC(_IOC_READ|_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))

// ioctls.h
#define TCGETS 0x5401
#define TCSETS 0x5402
#define TCSETSW 0x5403
#define TCSETSF 0x5404
#define TCGETA 0x5405
#define TCSETA 0x5406
#define TCSETAW 0x5407
#define TCSETAF 0x5408
#define TCSBRK 0x5409
#define TCXONC 0x540A
#define TCFLSH 0x540B
#define TIOCEXCL 0x540C
#define TIOCNXCL 0x540D
#define TIOCSCTTY 0x540E
#define TIOCGPGRP 0x540F
#define TIOCSPGRP 0x5410
#define TIOCOUTQ 0x5411
#define TIOCSTI 0x5412
#define TIOCGWINSZ 0x5413
#define TIOCSWINSZ 0x5414
#define TIOCMGET 0x5415
#define TIOCMBIS 0x5416
#define TIOCMBIC 0x5417
#define TIOCMSET 0x5418
#define TIOCGSOFTCAR 0x5419
#define TIOCSSOFTCAR 0x541A
#define FIONREAD 0x541B
#define TIOCINQ FIONREAD
#define TIOCLINUX 0x541C
#define TIOCCONS 0x541D
#define TIOCGSERIAL 0x541E
#define TIOCSSERIAL 0x541F
#define TIOCPKT 0x5420
#define FIONBIO 0x5421
#define TIOCNOTTY 0x5422
#define TIOCSETD 0x5423
#define TIOCGETD 0x5424
#define TCSBRKP 0x5425 // Needed for POSIX tcsendbreak()
#define TIOCSBRK 0x5427 // BSD compatibility
#define TIOCCBRK 0x5428 // BSD compatibility
#define TIOCGSID 0x5429 // Return the session ID of FD

// vt.h
#define MIN_NR_CONSOLES 1
#define MAX_NR_CONSOLES 63 // serial lines start at 64
#define VT_OPENQRY 0x5600 // find available vt

struct vt_mode {
    uint8_t mode; // vt mode
    uint8_t waitv; // if set, hang on writes if not active
    int16_t relsig; // signal to raise on release req
    int16_t acqsig; // signal to raise on acquisition
    int16_t frsig; // unused (set to 0)
};
#define VT_GETMODE 0x5601 // get mode of active vt
#define VT_SETMODE 0x5602 // set mode of active vt
#define VT_AUTO 0x00 // auto vt switching
#define VT_PROCESS 0x01 // process controls switching
#define VT_ACKACQ 0x02 // acknowledge switch

struct vt_stat {
    uint16_t v_active; // active vt
    uint16_t v_signal; // signal to send
    uint16_t v_state; // vt bitmask
};
#define VT_GETSTATE 0x5603 // get global vt state info
#define VT_SENDSIG 0x5604 // signal to send to bitmask of vts

#define VT_RELDISP 0x5605 // release display

#define VT_ACTIVATE 0x5606 // make vt active
#define VT_WAITACTIVE 0x5607 // wait for vt active
#define VT_DISALLOCATE 0x5608 // free memory associated to vt

struct vt_sizes {
    uint16_t v_rows; // number of rows
    uint16_t v_cols; // number of columns
    uint16_t v_scrollsize; // number of lines of scrollback
};
#define VT_RESIZE 0x5609 // set kernel's idea of screensize

struct vt_consize {
    uint16_t v_rows; // number of rows
    uint16_t v_cols; // number of columns
    uint16_t v_vlin; // number of pixel rows on screen
    uint16_t v_clin; // number of pixel rows per character
    uint16_t v_vcol; // number of pixel columns on screen
    uint16_t v_ccol; // number of pixel columns per character
};
#define VT_RESIZEX 0x560A // set kernel's idea of screensize + more
#define VT_LOCKSWITCH 0x560B // disallow vt switching
#define VT_UNLOCKSWITCH 0x560C // allow vt switching
#define VT_GETHIFONTMASK 0x560D // return hi font mask

#define VT_EVENT_SWITCH 0x0001 // Console switch
#define VT_EVENT_BLANK 0x0002 // Screen blank
#define VT_EVENT_UNBLANK 0x0004 // Screen unblank
#define VT_EVENT_RESIZE 0x0008 // Resize display
#define VT_MAX_EVENT 0x000F
struct vt_event {
    uint32_t event;
    uint32_t oldev; // Old console
    uint32_t newev; // New console (if changing)
    uint32_t __pad[4]; // Padding for expansion
};

#define VT_WAITEVENT 0x560E // Wait for an event

struct vt_setactivate {
    uint32_t console;
    struct vt_mode mode;
};

#define VT_SETACTIVATE 0x560F // Activate and set the mode of a console

// kd.h
#define KDSETMODE 0x4B3A // set text/graphics mode
#define KD_TEXT 0x00
#define KD_GRAPHICS 0x01
#define KD_TEXT0 0x02 // obsolete
#define KD_TEXT1 0x03 // obsolete
#define KDGETMODE 0x4B3B // get current mode

#define K_RAW 0x00
#define K_XLATE 0x01
#define K_MEDIUMRAW 0x02
#define K_UNICODE 0x03
#define K_OFF 0x04
#define KDGKBMODE 0x4B44 // gets current keyboard mode
#define KDSKBMODE 0x4B45 // sets current keyboard mode

#define K_METABIT 0x03
#define K_ESCPREFIX 0x04
#define KDGKBMETA 0x4B62 // gets meta key handling mode
#define KDSKBMETA 0x4B63 // sets meta key handling mode

struct kbentry {
    uint8_t kb_table;
    uint8_t kb_index;
    uint16_t kb_value;
};
#define K_NORMTAB 0x00
#define K_SHIFTTAB 0x01
#define K_ALTTAB 0x02
#define K_ALTSHIFTTAB 0x03

#define KDGKBENT 0x4B46 // gets one entry in translation table
#define KDSKBENT 0x4B47 // sets one entry in translation table

// signalfd.h
#define SFD_CLOEXEC 02000000
#define SFD_NONBLOCK 00004000

struct signalfd_siginfo {
    uint32_t ssi_signo;
    int32_t ssi_errno;
    int32_t ssi_code;
    uint32_t ssi_pid;
    uint32_t ssi_uid;
    int32_t ssi_fd;
    uint32_t ssi_tid;
    uint32_t ssi_band;
    uint32_t ssi_overrun;
    uint32_t ssi_trapno;
    int32_t ssi_status;
    int32_t ssi_int;
    uint64_t ssi_ptr;
    uint64_t ssi_utime;
    uint64_t ssi_stime;
    uint64_t ssi_addr;
    uint16_t ssi_addr_lsb;
    uint16_t __pad2;
    int32_t ssi_syscall;
    uint64_t ssi_call_addr;
    uint32_t ssi_arch;
    uint8_t __pad[28];
};

// drm.h
#define DRM_IOCTL_BASE 'd'
#define DRM_IO(nr) _IO(DRM_IOCTL_BASE,nr)
#define DRM_IOR(nr,type) _IOR(DRM_IOCTL_BASE,nr,type)
#define DRM_IOW(nr,type) _IOW(DRM_IOCTL_BASE,nr,type)
#define DRM_IOWR(nr,type) _IOWR(DRM_IOCTL_BASE,nr,type)

#define DRM_IOCTL_VERSION DRM_IOWR(0x00, struct drm_version)
#define DRM_IOCTL_GET_UNIQUE DRM_IOWR(0x01, struct drm_unique)
#define DRM_IOCTL_GET_MAGIC DRM_IOR(0x02, struct drm_auth)
#define DRM_IOCTL_IRQ_BUSID DRM_IOWR(0x03, struct drm_irq_busid)
#define DRM_IOCTL_GET_MAP DRM_IOWR(0x04, struct drm_map)
#define DRM_IOCTL_GET_CLIENT DRM_IOWR(0x05, struct drm_client)
#define DRM_IOCTL_GET_STATS DRM_IOR(0x06, struct drm_stats)
#define DRM_IOCTL_SET_VERSION DRM_IOWR(0x07, struct drm_set_version)
#define DRM_IOCTL_MODESET_CTL DRM_IOW(0x08, struct drm_modeset_ctl)
#define DRM_IOCTL_GEM_CLOSE DRM_IOW (0x09, struct drm_gem_close)
#define DRM_IOCTL_GEM_FLINK DRM_IOWR(0x0a, struct drm_gem_flink)
#define DRM_IOCTL_GEM_OPEN DRM_IOWR(0x0b, struct drm_gem_open)
#define DRM_IOCTL_GET_CAP DRM_IOWR(0x0c, struct drm_get_cap)
#define DRM_IOCTL_SET_CLIENT_CAP DRM_IOW(0x0d, struct drm_set_client_cap)

#define DRM_IOCTL_SET_UNIQUE DRM_IOW(0x10, struct drm_unique)
#define DRM_IOCTL_AUTH_MAGIC DRM_IOW(0x11, struct drm_auth)
#define DRM_IOCTL_BLOCK DRM_IOWR(0x12, struct drm_block)
#define DRM_IOCTL_UNBLOCK DRM_IOWR(0x13, struct drm_block)
#define DRM_IOCTL_CONTROL DRM_IOW(0x14, struct drm_control)
#define DRM_IOCTL_ADD_MAP DRM_IOWR(0x15, struct drm_map)
#define DRM_IOCTL_ADD_BUFS DRM_IOWR(0x16, struct drm_buf_desc)
#define DRM_IOCTL_MARK_BUFS DRM_IOW(0x17, struct drm_buf_desc)
#define DRM_IOCTL_INFO_BUFS DRM_IOWR(0x18, struct drm_buf_info)
#define DRM_IOCTL_MAP_BUFS DRM_IOWR(0x19, struct drm_buf_map)
#define DRM_IOCTL_FREE_BUFS DRM_IOW(0x1a, struct drm_buf_free)

#define DRM_IOCTL_RM_MAP DRM_IOW(0x1b, struct drm_map)

#define DRM_IOCTL_SET_SAREA_CTX DRM_IOW(0x1c, struct drm_ctx_priv_map)
#define DRM_IOCTL_GET_SAREA_CTX DRM_IOWR(0x1d, struct drm_ctx_priv_map)

#define DRM_IOCTL_SET_MASTER DRM_IO(0x1e)
#define DRM_IOCTL_DROP_MASTER DRM_IO(0x1f)

#define DRM_IOCTL_ADD_CTX DRM_IOWR(0x20, struct drm_ctx)
#define DRM_IOCTL_RM_CTX DRM_IOWR(0x21, struct drm_ctx)
#define DRM_IOCTL_MOD_CTX DRM_IOW(0x22, struct drm_ctx)
#define DRM_IOCTL_GET_CTX DRM_IOWR(0x23, struct drm_ctx)
#define DRM_IOCTL_SWITCH_CTX DRM_IOW(0x24, struct drm_ctx)
#define DRM_IOCTL_NEW_CTX DRM_IOW(0x25, struct drm_ctx)
#define DRM_IOCTL_RES_CTX DRM_IOWR(0x26, struct drm_ctx_res)
#define DRM_IOCTL_ADD_DRAW DRM_IOWR(0x27, struct drm_draw)
#define DRM_IOCTL_RM_DRAW DRM_IOWR(0x28, struct drm_draw)
#define DRM_IOCTL_DMA DRM_IOWR(0x29, struct drm_dma)
#define DRM_IOCTL_LOCK DRM_IOW(0x2a, struct drm_lock)
#define DRM_IOCTL_UNLOCK DRM_IOW(0x2b, struct drm_lock)
#define DRM_IOCTL_FINISH DRM_IOW(0x2c, struct drm_lock)

#define DRM_IOCTL_PRIME_HANDLE_TO_FD DRM_IOWR(0x2d, struct drm_prime_handle)
#define DRM_IOCTL_PRIME_FD_TO_HANDLE DRM_IOWR(0x2e, struct drm_prime_handle)

#define DRM_IOCTL_AGP_ACQUIRE DRM_IO(0x30)
#define DRM_IOCTL_AGP_RELEASE DRM_IO(0x31)
#define DRM_IOCTL_AGP_ENABLE DRM_IOW(0x32, struct drm_agp_mode)
#define DRM_IOCTL_AGP_INFO DRM_IOR(0x33, struct drm_agp_info)
#define DRM_IOCTL_AGP_ALLOC DRM_IOWR(0x34, struct drm_agp_buffer)
#define DRM_IOCTL_AGP_FREE DRM_IOW(0x35, struct drm_agp_buffer)
#define DRM_IOCTL_AGP_BIND DRM_IOW(0x36, struct drm_agp_binding)
#define DRM_IOCTL_AGP_UNBIND DRM_IOW(0x37, struct drm_agp_binding)

#define DRM_IOCTL_SG_ALLOC DRM_IOWR(0x38, struct drm_scatter_gather)
#define DRM_IOCTL_SG_FREE DRM_IOW(0x39, struct drm_scatter_gather)

#define DRM_IOCTL_WAIT_VBLANK DRM_IOWR(0x3a, union drm_wait_vblank)

#define DRM_IOCTL_CRTC_GET_SEQUENCE DRM_IOWR(0x3b, struct drm_crtc_get_sequence)
#define DRM_IOCTL_CRTC_QUEUE_SEQUENCE DRM_IOWR(0x3c, struct drm_crtc_queue_sequence)

#define DRM_IOCTL_UPDATE_DRAW DRM_IOW(0x3f, struct drm_update_draw)

#define DRM_IOCTL_MODE_GETRESOURCES DRM_IOWR(0xA0, struct drm_mode_card_res)
#define DRM_IOCTL_MODE_GETCRTC DRM_IOWR(0xA1, struct drm_mode_crtc)
#define DRM_IOCTL_MODE_SETCRTC DRM_IOWR(0xA2, struct drm_mode_crtc)
#define DRM_IOCTL_MODE_CURSOR DRM_IOWR(0xA3, struct drm_mode_cursor)
#define DRM_IOCTL_MODE_GETGAMMA DRM_IOWR(0xA4, struct drm_mode_crtc_lut)
#define DRM_IOCTL_MODE_SETGAMMA DRM_IOWR(0xA5, struct drm_mode_crtc_lut)
#define DRM_IOCTL_MODE_GETENCODER DRM_IOWR(0xA6, struct drm_mode_get_encoder)
#define DRM_IOCTL_MODE_GETCONNECTOR DRM_IOWR(0xA7, struct drm_mode_get_connector)

#define DRM_IOCTL_MODE_GETPROPERTY DRM_IOWR(0xAA, struct drm_mode_get_property)
#define DRM_IOCTL_MODE_SETPROPERTY DRM_IOWR(0xAB, struct drm_mode_connector_set_property)
#define DRM_IOCTL_MODE_GETPROPBLOB DRM_IOWR(0xAC, struct drm_mode_get_blob)
#define DRM_IOCTL_MODE_GETFB DRM_IOWR(0xAD, struct drm_mode_fb_cmd)
#define DRM_IOCTL_MODE_ADDFB DRM_IOWR(0xAE, struct drm_mode_fb_cmd)
/**
 * DRM_IOCTL_MODE_RMFB - Remove a framebuffer.
 *
 * This removes a framebuffer previously added via ADDFB/ADDFB2. The IOCTL
 * argument is a framebuffer object ID.
 *
 * Warning: removing a framebuffer currently in-use on an enabled plane will
 * disable that plane. The CRTC the plane is linked to may also be disabled
 * (depending on driver capabilities).
 */
#define DRM_IOCTL_MODE_RMFB DRM_IOWR(0xAF, unsigned int)
#define DRM_IOCTL_MODE_PAGE_FLIP DRM_IOWR(0xB0, struct drm_mode_crtc_page_flip)
#define DRM_IOCTL_MODE_DIRTYFB DRM_IOWR(0xB1, struct drm_mode_fb_dirty_cmd)

#define DRM_IOCTL_MODE_CREATE_DUMB DRM_IOWR(0xB2, struct drm_mode_create_dumb)
#define DRM_IOCTL_MODE_MAP_DUMB DRM_IOWR(0xB3, struct drm_mode_map_dumb)
#define DRM_IOCTL_MODE_DESTROY_DUMB DRM_IOWR(0xB4, struct drm_mode_destroy_dumb)
#define DRM_IOCTL_MODE_GETPLANERESOURCES DRM_IOWR(0xB5, struct drm_mode_get_plane_res)
#define DRM_IOCTL_MODE_GETPLANE DRM_IOWR(0xB6, struct drm_mode_get_plane)
#define DRM_IOCTL_MODE_SETPLANE DRM_IOWR(0xB7, struct drm_mode_set_plane)
#define DRM_IOCTL_MODE_ADDFB2 DRM_IOWR(0xB8, struct drm_mode_fb_cmd2)
#define DRM_IOCTL_MODE_OBJ_GETPROPERTIES DRM_IOWR(0xB9, struct drm_mode_obj_get_properties)
#define DRM_IOCTL_MODE_OBJ_SETPROPERTY DRM_IOWR(0xBA, struct drm_mode_obj_set_property)
#define DRM_IOCTL_MODE_CURSOR2 DRM_IOWR(0xBB, struct drm_mode_cursor2)
#define DRM_IOCTL_MODE_ATOMIC DRM_IOWR(0xBC, struct drm_mode_atomic)
#define DRM_IOCTL_MODE_CREATEPROPBLOB DRM_IOWR(0xBD, struct drm_mode_create_blob)
#define DRM_IOCTL_MODE_DESTROYPROPBLOB DRM_IOWR(0xBE, struct drm_mode_destroy_blob)

#define DRM_IOCTL_SYNCOBJ_CREATE DRM_IOWR(0xBF, struct drm_syncobj_create)
#define DRM_IOCTL_SYNCOBJ_DESTROY DRM_IOWR(0xC0, struct drm_syncobj_destroy)
#define DRM_IOCTL_SYNCOBJ_HANDLE_TO_FD DRM_IOWR(0xC1, struct drm_syncobj_handle)
#define DRM_IOCTL_SYNCOBJ_FD_TO_HANDLE DRM_IOWR(0xC2, struct drm_syncobj_handle)
#define DRM_IOCTL_SYNCOBJ_WAIT DRM_IOWR(0xC3, struct drm_syncobj_wait)
#define DRM_IOCTL_SYNCOBJ_RESET DRM_IOWR(0xC4, struct drm_syncobj_array)
#define DRM_IOCTL_SYNCOBJ_SIGNAL DRM_IOWR(0xC5, struct drm_syncobj_array)

#define DRM_IOCTL_MODE_CREATE_LEASE DRM_IOWR(0xC6, struct drm_mode_create_lease)
#define DRM_IOCTL_MODE_LIST_LESSEES DRM_IOWR(0xC7, struct drm_mode_list_lessees)
#define DRM_IOCTL_MODE_GET_LEASE DRM_IOWR(0xC8, struct drm_mode_get_lease)
#define DRM_IOCTL_MODE_REVOKE_LEASE DRM_IOWR(0xC9, struct drm_mode_revoke_lease)

#define DRM_IOCTL_SYNCOBJ_TIMELINE_WAIT DRM_IOWR(0xCA, struct drm_syncobj_timeline_wait)
#define DRM_IOCTL_SYNCOBJ_QUERY DRM_IOWR(0xCB, struct drm_syncobj_timeline_array)
#define DRM_IOCTL_SYNCOBJ_TRANSFER DRM_IOWR(0xCC, struct drm_syncobj_transfer)
#define DRM_IOCTL_SYNCOBJ_TIMELINE_SIGNAL DRM_IOWR(0xCD, struct drm_syncobj_timeline_array)

#define DRM_IOCTL_MODE_GETFB2 DRM_IOWR(0xCE, struct drm_mode_fb_cmd2)

// drm_mode.h
#define DRM_CONNECTOR_NAME_LEN 32
#define DRM_DISPLAY_MODE_LEN 32
#define DRM_PROP_NAME_LEN 32

#define DRM_MODE_TYPE_PREFERRED (1<<3)
#define DRM_MODE_TYPE_USERDEF (1<<5)
#define DRM_MODE_TYPE_DRIVER (1<<6)

#define DRM_MODE_TYPE_ALL (DRM_MODE_TYPE_PREFERRED | DRM_MODE_TYPE_USERDEF | DRM_MODE_TYPE_DRIVER)

// Video mode flags
#define DRM_MODE_FLAG_PHSYNC (1<<0)
#define DRM_MODE_FLAG_NHSYNC (1<<1)
#define DRM_MODE_FLAG_PVSYNC (1<<2)
#define DRM_MODE_FLAG_NVSYNC (1<<3)
#define DRM_MODE_FLAG_INTERLACE (1<<4)
#define DRM_MODE_FLAG_DBLSCAN (1<<5)
#define DRM_MODE_FLAG_CSYNC (1<<6)
#define DRM_MODE_FLAG_PCSYNC (1<<7)
#define DRM_MODE_FLAG_NCSYNC (1<<8)
#define DRM_MODE_FLAG_HSKEW (1<<9) // hskew provided
#define DRM_MODE_FLAG_DBLCLK (1<<12)
#define DRM_MODE_FLAG_CLKDIV2 (1<<13)

#define DRM_MODE_FLAG_3D_MASK (0x1f<<14)
#define DRM_MODE_FLAG_3D_NONE (0<<14)
#define DRM_MODE_FLAG_3D_FRAME_PACKING (1<<14)
#define DRM_MODE_FLAG_3D_FIELD_ALTERNATIVE (2<<14)
#define DRM_MODE_FLAG_3D_LINE_ALTERNATIVE (3<<14)
#define DRM_MODE_FLAG_3D_SIDE_BY_SIDE_FULL (4<<14)
#define DRM_MODE_FLAG_3D_L_DEPTH (5<<14)
#define DRM_MODE_FLAG_3D_L_DEPTH_GFX_GFX_DEPTH (6<<14)
#define DRM_MODE_FLAG_3D_TOP_AND_BOTTOM (7<<14)
#define DRM_MODE_FLAG_3D_SIDE_BY_SIDE_HALF (8<<14)

// Picture aspect ratio options
#define DRM_MODE_PICTURE_ASPECT_NONE 0
#define DRM_MODE_PICTURE_ASPECT_4_3 1
#define DRM_MODE_PICTURE_ASPECT_16_9 2
#define DRM_MODE_PICTURE_ASPECT_64_27 3
#define DRM_MODE_PICTURE_ASPECT_256_135 4

// Content type options
#define DRM_MODE_CONTENT_TYPE_NO_DATA 0
#define DRM_MODE_CONTENT_TYPE_GRAPHICS 1
#define DRM_MODE_CONTENT_TYPE_PHOTO 2
#define DRM_MODE_CONTENT_TYPE_CINEMA 3
#define DRM_MODE_CONTENT_TYPE_GAME 4

// Aspect ratio flag bitmask (4 bits 22:19)
#define DRM_MODE_FLAG_PIC_AR_MASK (0x0F<<19)
#define DRM_MODE_FLAG_PIC_AR_NONE (DRM_MODE_PICTURE_ASPECT_NONE<<19)
#define DRM_MODE_FLAG_PIC_AR_4_3 (DRM_MODE_PICTURE_ASPECT_4_3<<19)
#define DRM_MODE_FLAG_PIC_AR_16_9 (DRM_MODE_PICTURE_ASPECT_16_9<<19)
#define DRM_MODE_FLAG_PIC_AR_64_27 (DRM_MODE_PICTURE_ASPECT_64_27<<19)
#define DRM_MODE_FLAG_PIC_AR_256_135 (DRM_MODE_PICTURE_ASPECT_256_135<<19)

#define DRM_MODE_FLAG_ALL ( \
    DRM_MODE_FLAG_PHSYNC | \
    DRM_MODE_FLAG_NHSYNC | \
    DRM_MODE_FLAG_PVSYNC | \
    DRM_MODE_FLAG_NVSYNC | \
    DRM_MODE_FLAG_INTERLACE | \
    DRM_MODE_FLAG_DBLSCAN | \
    DRM_MODE_FLAG_CSYNC | \
    DRM_MODE_FLAG_PCSYNC | \
    DRM_MODE_FLAG_NCSYNC | \
    DRM_MODE_FLAG_HSKEW | \
    DRM_MODE_FLAG_DBLCLK | \
    DRM_MODE_FLAG_CLKDIV2 | \
    DRM_MODE_FLAG_3D_MASK \
)

// DPMS flags
#define DRM_MODE_DPMS_ON 0
#define DRM_MODE_DPMS_STANDBY 1
#define DRM_MODE_DPMS_SUSPEND 2
#define DRM_MODE_DPMS_OFF 3

// Scaling mode options
#define DRM_MODE_SCALE_NONE 0 // Unmodified timing (display or software can still scale)
#define DRM_MODE_SCALE_FULLSCREEN 1 // Full screen, ignore aspect
#define DRM_MODE_SCALE_CENTER 2 // Centered, no scaling
#define DRM_MODE_SCALE_ASPECT 3 // Full screen, preserve aspect

// Dithering mode options
#define DRM_MODE_DITHERING_OFF 0
#define DRM_MODE_DITHERING_ON 1
#define DRM_MODE_DITHERING_AUTO 2

// Dirty info options
#define DRM_MODE_DIRTY_OFF      0
#define DRM_MODE_DIRTY_ON       1
#define DRM_MODE_DIRTY_ANNOTATE 2

// Link Status options
#define DRM_MODE_LINK_STATUS_GOOD 0
#define DRM_MODE_LINK_STATUS_BAD 1

/*
 * DRM_MODE_ROTATE_<degrees>
 *
 * Signals that a drm plane is been rotated <degrees> degrees in counter
 * clockwise direction.
 *
 * This define is provided as a convenience, looking up the property id
 * using the name->prop id lookup is the preferred method.
 */
#define DRM_MODE_ROTATE_0       (1<<0)
#define DRM_MODE_ROTATE_90      (1<<1)
#define DRM_MODE_ROTATE_180     (1<<2)
#define DRM_MODE_ROTATE_270     (1<<3)

/*
 * DRM_MODE_ROTATE_MASK
 *
 * Bitmask used to look for drm plane rotations.
 */
#define DRM_MODE_ROTATE_MASK (DRM_MODE_ROTATE_0  | DRM_MODE_ROTATE_90  | DRM_MODE_ROTATE_180 | DRM_MODE_ROTATE_270)

/*
 * DRM_MODE_REFLECT_<axis>
 *
 * Signals that the contents of a drm plane is reflected along the <axis> axis,
 * in the same way as mirroring.
 * See kerneldoc chapter "Plane Composition Properties" for more details.
 *
 * This define is provided as a convenience, looking up the property id
 * using the name->prop id lookup is the preferred method.
 */
#define DRM_MODE_REFLECT_X      (1<<4)
#define DRM_MODE_REFLECT_Y      (1<<5)

/*
 * DRM_MODE_REFLECT_MASK
 *
 * Bitmask used to look for drm plane reflections.
 */
#define DRM_MODE_REFLECT_MASK (DRM_MODE_REFLECT_X | DRM_MODE_REFLECT_Y)

// Content Protection Flags
#define DRM_MODE_CONTENT_PROTECTION_UNDESIRED 0
#define DRM_MODE_CONTENT_PROTECTION_DESIRED     1
#define DRM_MODE_CONTENT_PROTECTION_ENABLED     2

/**
 * struct drm_mode_modeinfo - Display mode information.
 * @clock: pixel clock in kHz
 * @hdisplay: horizontal display size
 * @hsync_start: horizontal sync start
 * @hsync_end: horizontal sync end
 * @htotal: horizontal total size
 * @hskew: horizontal skew
 * @vdisplay: vertical display size
 * @vsync_start: vertical sync start
 * @vsync_end: vertical sync end
 * @vtotal: vertical total size
 * @vscan: vertical scan
 * @vrefresh: approximate vertical refresh rate in Hz
 * @flags: bitmask of misc. flags, see DRM_MODE_FLAG_* defines
 * @type: bitmask of type flags, see DRM_MODE_TYPE_* defines
 * @name: string describing the mode resolution
 *
 * This is the user-space API display mode information structure. For the
 * kernel version see struct drm_display_mode.
 */
struct drm_mode_modeinfo {
    uint32_t clock;
    uint16_t hdisplay;
    uint16_t hsync_start;
    uint16_t hsync_end;
    uint16_t htotal;
    uint16_t hskew;
    uint16_t vdisplay;
    uint16_t vsync_start;
    uint16_t vsync_end;
    uint16_t vtotal;
    uint16_t vscan;

    int32_t vrefresh;

    uint32_t flags;
    uint32_t type;
    char name[DRM_DISPLAY_MODE_LEN];
};

struct drm_mode_card_res {
    uint32_t *fb_id_ptr;
    uint32_t *crtc_id_ptr;
    uint32_t *connector_id_ptr;
    uint32_t *encoder_id_ptr;
    uint32_t count_fbs;
    uint32_t count_crtcs;
    uint32_t count_connectors;
    uint32_t count_encoders;
    uint32_t min_width;
    uint32_t max_width;
    uint32_t min_height;
    uint32_t max_height;
};

struct drm_mode_crtc {
    uint32_t *set_connectors_ptr;
    uint32_t count_connectors;

    uint32_t crtc_id; // Id
    uint32_t fb_id; // Id of framebuffer

    uint32_t x; // x Position on the framebuffer
    uint32_t y; // y Position on the framebuffer

    uint32_t gamma_size;
    uint32_t mode_valid;
    struct drm_mode_modeinfo mode;
};

#define DRM_MODE_PRESENT_TOP_FIELD (1<<0)
#define DRM_MODE_PRESENT_BOTTOM_FIELD (1<<1)

// Planes blend with or override other bits on the CRTC
struct drm_mode_set_plane {
    uint32_t plane_id;
    uint32_t crtc_id;
    uint32_t fb_id; // fb object contains surface format type
    uint32_t flags; // see above flags

    // Signed dest location allows it to be partially off screen
    int32_t crtc_x;
    int32_t crtc_y;
    uint32_t crtc_w;
    uint32_t crtc_h;

    // Source values are 16.16 fixed point
    uint32_t src_x;
    uint32_t src_y;
    uint32_t src_h;
    uint32_t src_w;
};

/**
 * struct drm_mode_get_plane - Get plane metadata.
 *
 * Userspace can perform a GETPLANE ioctl to retrieve information about a
 * plane.
 *
 * To retrieve the number of formats supported, set @count_format_types to zero
 * and call the ioctl. @count_format_types will be updated with the value.
 *
 * To retrieve these formats, allocate an array with the memory needed to store
 * @count_format_types formats. Point @format_type_ptr to this array and call
 * the ioctl again (with @count_format_types still set to the value returned in
 * the first ioctl call).
 */
struct drm_mode_get_plane {
    /**
     * @plane_id: Object ID of the plane whose information should be
     * retrieved. Set by caller.
     */
    uint32_t plane_id;

    /** @crtc_id: Object ID of the current CRTC. */
    uint32_t crtc_id;
    /** @fb_id: Object ID of the current fb. */
    uint32_t fb_id;

    /**
     * @possible_crtcs: Bitmask of CRTC's compatible with the plane. CRTC's
     * are created and they receive an index, which corresponds to their
     * position in the bitmask. Bit N corresponds to
     * :ref:`CRTC index<crtc_index>` N.
     */
    uint32_t possible_crtcs;
    /** @gamma_size: Never used. */
    uint32_t gamma_size;

    /** @count_format_types: Number of formats. */
    uint32_t count_format_types;
    /**
     * @format_type_ptr: Pointer to ``uint32_t`` array of formats that are
     * supported by the plane. These formats do not require modifiers.
     */
    uint64_t format_type_ptr;
};

struct drm_mode_get_plane_res {
    uint64_t plane_id_ptr;
    uint32_t count_planes;
    uint32_t __pad;
};

#define DRM_MODE_ENCODER_NONE 0
#define DRM_MODE_ENCODER_DAC 1
#define DRM_MODE_ENCODER_TMDS 2
#define DRM_MODE_ENCODER_LVDS 3
#define DRM_MODE_ENCODER_TVDAC 4
#define DRM_MODE_ENCODER_VIRTUAL 5
#define DRM_MODE_ENCODER_DSI 6
#define DRM_MODE_ENCODER_DPMST 7
#define DRM_MODE_ENCODER_DPI 8

struct drm_mode_get_encoder {
    uint32_t encoder_id;
    uint32_t encoder_type;

    uint32_t crtc_id; // Id of crtc

    uint32_t possible_crtcs;
    uint32_t possible_clones;
};

/* This is for connectors with multiple signal types. */
/* Try to match DRM_MODE_CONNECTOR_X as closely as possible. */
enum drm_mode_subconnector {
    DRM_MODE_SUBCONNECTOR_Automatic = 0, // DVI-I, TV
    DRM_MODE_SUBCONNECTOR_Unknown = 0, // DVI-I, TV, DP
    DRM_MODE_SUBCONNECTOR_VGA = 1, // DP */
    DRM_MODE_SUBCONNECTOR_DVID = 3, // DVI-I, DP
    DRM_MODE_SUBCONNECTOR_DVIA = 4, // DVI-I
    DRM_MODE_SUBCONNECTOR_Composite = 5, // TV
    DRM_MODE_SUBCONNECTOR_SVIDEO = 6, // TV
    DRM_MODE_SUBCONNECTOR_Component = 8, // TV
    DRM_MODE_SUBCONNECTOR_SCART = 9, // TV
    DRM_MODE_SUBCONNECTOR_DisplayPort = 10, // DP
    DRM_MODE_SUBCONNECTOR_HDMIA = 11, // DP
    DRM_MODE_SUBCONNECTOR_Native = 15, // DP
    DRM_MODE_SUBCONNECTOR_Wireless = 18, // DP
};

#define DRM_MODE_CONNECTOR_Unknown 0
#define DRM_MODE_CONNECTOR_VGA 1
#define DRM_MODE_CONNECTOR_DVII 2
#define DRM_MODE_CONNECTOR_DVID 3
#define DRM_MODE_CONNECTOR_DVIA 4
#define DRM_MODE_CONNECTOR_Composite 5
#define DRM_MODE_CONNECTOR_SVIDEO 6
#define DRM_MODE_CONNECTOR_LVDS 7
#define DRM_MODE_CONNECTOR_Component 8
#define DRM_MODE_CONNECTOR_9PinDIN 9
#define DRM_MODE_CONNECTOR_DisplayPort 10
#define DRM_MODE_CONNECTOR_HDMIA 11
#define DRM_MODE_CONNECTOR_HDMIB 12
#define DRM_MODE_CONNECTOR_TV 13
#define DRM_MODE_CONNECTOR_eDP 14
#define DRM_MODE_CONNECTOR_VIRTUAL      15
#define DRM_MODE_CONNECTOR_DSI 16
#define DRM_MODE_CONNECTOR_DPI 17
#define DRM_MODE_CONNECTOR_WRITEBACK 18
#define DRM_MODE_CONNECTOR_SPI 19
#define DRM_MODE_CONNECTOR_USB 20

/**
 * struct drm_mode_get_connector - Get connector metadata.
 *
 * User-space can perform a GETCONNECTOR ioctl to retrieve information about a
 * connector. User-space is expected to retrieve encoders, modes and properties
 * by performing this ioctl at least twice: the first time to retrieve the
 * number of elements, the second time to retrieve the elements themselves.
 *
 * To retrieve the number of elements, set @count_props and @count_encoders to
 * zero, set @count_modes to 1, and set @modes_ptr to a temporary struct
 * drm_mode_modeinfo element.
 *
 * To retrieve the elements, allocate arrays for @encoders_ptr, @modes_ptr,
 * @props_ptr and @prop_values_ptr, then set @count_modes, @count_props and
 * @count_encoders to their capacity.
 *
 * Performing the ioctl only twice may be racy: the number of elements may have
 * changed with a hotplug event in-between the two ioctls. User-space is
 * expected to retry the last ioctl until the number of elements stabilizes.
 * The kernel won't fill any array which doesn't have the expected length.
 *
 * **Force-probing a connector**
 *
 * If the @count_modes field is set to zero and the DRM client is the current
 * DRM master, the kernel will perform a forced probe on the connector to
 * refresh the connector status, modes and EDID. A forced-probe can be slow,
 * might cause flickering and the ioctl will block.
 *
 * User-space needs to force-probe connectors to ensure their metadata is
 * up-to-date at startup and after receiving a hot-plug event. User-space
 * may perform a forced-probe when the user explicitly requests it. User-space
 * shouldn't perform a forced-probe in other situations.
 */
struct drm_mode_get_connector {
    /** @encoders_ptr: Pointer to ``uint32_t`` array of object IDs. */
    uint32_t *encoders_ptr;
    /** @modes_ptr: Pointer to struct drm_mode_modeinfo array. */
    struct drm_mode_modeinfo *modes_ptr;
    /** @props_ptr: Pointer to ``uint32_t`` array of property IDs. */
    uint32_t *props_ptr;
    /** @prop_values_ptr: Pointer to ``uint64_t`` array of property values. */
    uint64_t *prop_values_ptr;

    /** @count_modes: Number of modes. */
    int32_t count_modes;
    /** @count_props: Number of properties. */
    int32_t count_props;
    /** @count_encoders: Number of encoders. */
    int32_t count_encoders;

    /** @encoder_id: Object ID of the current encoder. */
    uint32_t encoder_id;
    /** @connector_id: Object ID of the connector. */
    uint32_t connector_id;
    /**
     * @connector_type: Type of the connector.
     *
     * See DRM_MODE_CONNECTOR_* defines.
     */
    uint32_t connector_type;
    /**
     * @connector_type_id: Type-specific connector number.
     *
     * This is not an object ID. This is a per-type connector number. Each
     * (type, type_id) combination is unique across all connectors of a DRM
     * device.
     */
    uint32_t connector_type_id;

    /**
     * @connection: Status of the connector.
     *
     * See enum drm_connector_status.
     */
    uint32_t connection;
    /** @mm_width: Width of the connected sink in millimeters. */
    uint32_t mm_width;
    /** @mm_height: Height of the connected sink in millimeters. */
    uint32_t mm_height;
    /**
     * @subpixel: Subpixel order of the connected sink.
     *
     * See enum subpixel_order.
     */
    uint32_t subpixel;

    /** @pad: Padding, must be zero. */
    uint32_t pad;
};

#define DRM_MODE_PROP_RANGE (1<<1)
#define DRM_MODE_PROP_IMMUTABLE (1<<2)
#define DRM_MODE_PROP_ENUM (1<<3) // enumerated type with text strings
#define DRM_MODE_PROP_BLOB (1<<4)
#define DRM_MODE_PROP_BITMASK (1<<5) // bitmask of enumerated types

// non-extended types: legacy bitmask, one bit per type:
#define DRM_MODE_PROP_LEGACY_TYPE (DRM_MODE_PROP_RANGE | DRM_MODE_PROP_ENUM | DRM_MODE_PROP_BLOB | DRM_MODE_PROP_BITMASK)

/* extended-types: rather than continue to consume a bit per type,
 * grab a chunk of the bits to use as integer type id.
 */
#define DRM_MODE_PROP_EXTENDED_TYPE 0x0000ffc0
#define DRM_MODE_PROP_TYPE(n) ((n) << 6)
#define DRM_MODE_PROP_OBJECT DRM_MODE_PROP_TYPE(1)
#define DRM_MODE_PROP_SIGNED_RANGE DRM_MODE_PROP_TYPE(2)

/* the PROP_ATOMIC flag is used to hide properties from userspace that
 * is not aware of atomic properties.  This is mostly to work around
 * older userspace (DDX drivers) that read/write each prop they find,
 * witout being aware that this could be triggering a lengthy modeset.
 */
#define DRM_MODE_PROP_ATOMIC 0x80000000

/**
 * struct drm_mode_property_enum - Description for an enum/bitfield entry.
 * @value: numeric value for this enum entry.
 * @name: symbolic name for this enum entry.
 *
 * See struct drm_property_enum for details.
 */
struct drm_mode_property_enum {
    uint64_t value;
    char name[DRM_PROP_NAME_LEN];
};

/**
 * struct drm_mode_get_property - Get property metadata.
 *
 * User-space can perform a GETPROPERTY ioctl to retrieve information about a
 * property. The same property may be attached to multiple objects, see
 * "Modeset Base Object Abstraction".
 *
 * The meaning of the @values_ptr field changes depending on the property type.
 * See &drm_property.flags for more details.
 *
 * The @enum_blob_ptr and @count_enum_blobs fields are only meaningful when the
 * property has the type &DRM_MODE_PROP_ENUM or &DRM_MODE_PROP_BITMASK. For
 * backwards compatibility, the kernel will always set @count_enum_blobs to
 * zero when the property has the type &DRM_MODE_PROP_BLOB. User-space must
 * ignore these two fields if the property has a different type.
 *
 * User-space is expected to retrieve values and enums by performing this ioctl
 * at least twice: the first time to retrieve the number of elements, the
 * second time to retrieve the elements themselves.
 *
 * To retrieve the number of elements, set @count_values and @count_enum_blobs
 * to zero, then call the ioctl. @count_values will be updated with the number
 * of elements. If the property has the type &DRM_MODE_PROP_ENUM or
 * &DRM_MODE_PROP_BITMASK, @count_enum_blobs will be updated as well.
 *
 * To retrieve the elements themselves, allocate an array for @values_ptr and
 * set @count_values to its capacity. If the property has the type
 * &DRM_MODE_PROP_ENUM or &DRM_MODE_PROP_BITMASK, allocate an array for
 * @enum_blob_ptr and set @count_enum_blobs to its capacity. Calling the ioctl
 * again will fill the arrays.
 */
struct drm_mode_get_property {
    /** @values_ptr: Pointer to a ``uint64_t`` array. */
    uint64_t values_ptr;
    /** @enum_blob_ptr: Pointer to a struct drm_mode_property_enum array. */
    uint64_t enum_blob_ptr;

    /**
     * @prop_id: Object ID of the property which should be retrieved. Set
     * by the caller.
     */
    uint32_t prop_id;
    /**
     * @flags: ``DRM_MODE_PROP_*`` bitfield. See &drm_property.flags for
     * a definition of the flags.
     */
    uint32_t flags;
    /**
     * @name: Symbolic property name. User-space should use this field to
     * recognize properties.
     */
    char name[DRM_PROP_NAME_LEN];

    /** @count_values: Number of elements in @values_ptr. */
    uint32_t count_values;
    /** @count_enum_blobs: Number of elements in @enum_blob_ptr. */
    uint32_t count_enum_blobs;
};

struct drm_mode_connector_set_property {
    uint64_t value;
    uint32_t prop_id;
    uint32_t connector_id;
};

#define DRM_MODE_OBJECT_CRTC 0xcccccccc
#define DRM_MODE_OBJECT_CONNECTOR 0xc0c0c0c0
#define DRM_MODE_OBJECT_ENCODER 0xe0e0e0e0
#define DRM_MODE_OBJECT_MODE 0xdededede
#define DRM_MODE_OBJECT_PROPERTY 0xb0b0b0b0
#define DRM_MODE_OBJECT_FB 0xfbfbfbfb
#define DRM_MODE_OBJECT_BLOB 0xbbbbbbbb
#define DRM_MODE_OBJECT_PLANE 0xeeeeeeee
#define DRM_MODE_OBJECT_ANY 0

struct drm_mode_obj_get_properties {
    uint64_t props_ptr;
    uint64_t prop_values_ptr;
    uint32_t count_props;
    uint32_t obj_id;
    uint32_t obj_type;
    uint32_t __pad;
};

struct drm_mode_obj_set_property {
    uint64_t value;
    uint32_t prop_id;
    uint32_t obj_id;
    uint32_t obj_type;
    uint32_t __pad;
};

struct drm_mode_get_blob {
    uint32_t blob_id;
    uint32_t length;
    uint64_t data;
};

struct drm_mode_fb_cmd {
    uint32_t fb_id;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint32_t bpp;
    uint32_t depth;

    uint32_t handle; // driver specific handle
};

#define DRM_MODE_FB_INTERLACED (1<<0) // for interlaced framebuffers
#define DRM_MODE_FB_MODIFIERS (1<<1) // enables ->modifer[]

struct drm_mode_fb_cmd2 {
    uint32_t fb_id;
    uint32_t width;
    uint32_t height;
    uint32_t pixel_format; /* fourcc code from drm_fourcc.h */
    uint32_t flags; /* see above flags */

    /*
     * In case of planar formats, this ioctl allows up to 4
     * buffer objects with offsets and pitches per plane.
     * The pitch and offset order is dictated by the fourcc,
     * e.g. NV12 (https://fourcc.org/yuv.php#NV12) is described as:
     *
     *   YUV 4:2:0 image with a plane of 8 bit Y samples
     *   followed by an interleaved U/V plane containing
     *   8 bit 2x2 subsampled colour difference samples.
     *
     * So it would consist of Y as offsets[0] and UV as
     * offsets[1].  Note that offsets[0] will generally
     * be 0 (but this is not required).
     *
     * To accommodate tiled, compressed, etc formats, a
     * modifier can be specified.  The default value of zero
     * indicates "native" format as specified by the fourcc.
     * Vendor specific modifier token.  Note that even though
     * it looks like we have a modifier per-plane, we in fact
     * do not. The modifier for each plane must be identical.
     * Thus all combinations of different data layouts for
     * multi plane formats must be enumerated as separate
     * modifiers.
     */
    uint32_t handles[4];
    uint32_t pitches[4]; // pitch for each plane
    uint32_t offsets[4]; // offset of each plane
    uint32_t __pad;
    uint64_t modifier[4]; // ie, tiling, compress
};

#define DRM_MODE_FB_DIRTY_ANNOTATE_COPY 0x01
#define DRM_MODE_FB_DIRTY_ANNOTATE_FILL 0x02
#define DRM_MODE_FB_DIRTY_FLAGS         0x03

#define DRM_MODE_FB_DIRTY_MAX_CLIPS     256

/*
 * Mark a region of a framebuffer as dirty.
 *
 * Some hardware does not automatically update display contents
 * as a hardware or software draw to a framebuffer. This ioctl
 * allows userspace to tell the kernel and the hardware what
 * regions of the framebuffer have changed.
 *
 * The kernel or hardware is free to update more then just the
 * region specified by the clip rects. The kernel or hardware
 * may also delay and/or coalesce several calls to dirty into a
 * single update.
 *
 * Userspace may annotate the updates, the annotates are a
 * promise made by the caller that the change is either a copy
 * of pixels or a fill of a single color in the region specified.
 *
 * If the DRM_MODE_FB_DIRTY_ANNOTATE_COPY flag is given then
 * the number of updated regions are half of num_clips given,
 * where the clip rects are paired in src and dst. The width and
 * height of each one of the pairs must match.
 *
 * If the DRM_MODE_FB_DIRTY_ANNOTATE_FILL flag is given the caller
 * promises that the region specified of the clip rects is filled
 * completely with a single color as given in the color argument.
 */

struct drm_mode_fb_dirty_cmd {
    uint32_t fb_id;
    uint32_t flags;
    uint32_t color;
    uint32_t num_clips;
    uint64_t clips_ptr;
};

struct drm_mode_mode_cmd {
    uint32_t connector_id;
    struct drm_mode_modeinfo mode;
};

#define DRM_MODE_CURSOR_BO 0x01
#define DRM_MODE_CURSOR_MOVE 0x02
#define DRM_MODE_CURSOR_FLAGS 0x03

/*
 * depending on the value in flags different members are used.
 *
 * CURSOR_BO uses
 *    crtc_id
 *    width
 *    height
 *    handle - if 0 turns the cursor off
 *
 * CURSOR_MOVE uses
 *    crtc_id
 *    x
 *    y
 */
struct drm_mode_cursor {
    uint32_t flags;
    uint32_t crtc_id;
    int32_t x;
    int32_t y;
    uint32_t width;
    uint32_t height;

    uint32_t handle; // driver specific handle
};

struct drm_mode_cursor2 {
    uint32_t flags;
    uint32_t crtc_id;
    int32_t x;
    int32_t y;
    uint32_t width;
    uint32_t height;

    uint32_t handle; // driver specific handle
    int32_t hot_x;
    int32_t hot_y;
};

struct drm_mode_crtc_lut {
    uint32_t crtc_id;
    uint32_t gamma_size;

    // pointers to arrays
    uint64_t red;
    uint64_t green;
    uint64_t blue;
};

struct drm_color_ctm {
    /*
     * Conversion matrix in S31.32 sign-magnitude
     * (not two's complement!) format.
     */
    uint64_t matrix[9];
};

struct drm_color_lut {
    /*
     * Values are mapped linearly to 0.0 - 1.0 range, with 0x0 == 0.0 and
     * 0xffff == 1.0.
     */
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t reserved;
};

/**
 * struct hdr_metadata_infoframe - HDR Metadata Infoframe Data.
 *
 * HDR Metadata Infoframe as per CTA 861.G spec. This is expected
 * to match exactly with the spec.
 *
 * Userspace is expected to pass the metadata information as per
 * the format described in this structure.
 */
struct hdr_metadata_infoframe {
    /**
     * @eotf: Electro-Optical Transfer Function (EOTF)
     * used in the stream.
     */
    uint8_t eotf;
    /**
     * @metadata_type: Static_Metadata_Descriptor_ID.
     */
    uint8_t metadata_type;
    /**
     * @display_primaries: Color Primaries of the Data.
     * These are coded as unsigned 16-bit values in units of
     * 0.00002, where 0x0000 represents zero and 0xC350
     * represents 1.0000.
     * @display_primaries.x: X cordinate of color primary.
     * @display_primaries.y: Y cordinate of color primary.
     */
    struct {
        uint16_t x, y;
    } display_primaries[3];
    /**
     * @white_point: White Point of Colorspace Data.
     * These are coded as unsigned 16-bit values in units of
     * 0.00002, where 0x0000 represents zero and 0xC350
     * represents 1.0000.
     * @white_point.x: X cordinate of whitepoint of color primary.
     * @white_point.y: Y cordinate of whitepoint of color primary.
     */
    struct {
        uint16_t x, y;
    } white_point;
    /**
     * @max_display_mastering_luminance: Max Mastering Display Luminance.
     * This value is coded as an unsigned 16-bit value in units of 1 cd/m2,
     * where 0x0001 represents 1 cd/m2 and 0xFFFF represents 65535 cd/m2.
     */
    uint16_t max_display_mastering_luminance;
    /**
     * @min_display_mastering_luminance: Min Mastering Display Luminance.
     * This value is coded as an unsigned 16-bit value in units of
     * 0.0001 cd/m2, where 0x0001 represents 0.0001 cd/m2 and 0xFFFF
     * represents 6.5535 cd/m2.
     */
    uint16_t min_display_mastering_luminance;
    /**
     * @max_cll: Max Content Light Level.
     * This value is coded as an unsigned 16-bit value in units of 1 cd/m2,
     * where 0x0001 represents 1 cd/m2 and 0xFFFF represents 65535 cd/m2.
     */
    uint16_t max_cll;
    /**
     * @max_fall: Max Frame Average Light Level.
     * This value is coded as an unsigned 16-bit value in units of 1 cd/m2,
     * where 0x0001 represents 1 cd/m2 and 0xFFFF represents 65535 cd/m2.
     */
    uint16_t max_fall;
};

/**
 * struct hdr_output_metadata - HDR output metadata
 *
 * Metadata Information to be passed from userspace
 */
struct hdr_output_metadata {
    /**
     * @metadata_type: Static_Metadata_Descriptor_ID.
     */
    uint32_t metadata_type;
    /**
     * @hdmi_metadata_type1: HDR Metadata Infoframe.
     */
    union {
        struct hdr_metadata_infoframe hdmi_metadata_type1;
    };
    uint8_t __pad[2];
};

#define DRM_MODE_PAGE_FLIP_EVENT 0x01
#define DRM_MODE_PAGE_FLIP_ASYNC 0x02
#define DRM_MODE_PAGE_FLIP_TARGET_ABSOLUTE 0x4
#define DRM_MODE_PAGE_FLIP_TARGET_RELATIVE 0x8
#define DRM_MODE_PAGE_FLIP_TARGET (DRM_MODE_PAGE_FLIP_TARGET_ABSOLUTE | DRM_MODE_PAGE_FLIP_TARGET_RELATIVE)
#define DRM_MODE_PAGE_FLIP_FLAGS (DRM_MODE_PAGE_FLIP_EVENT | DRM_MODE_PAGE_FLIP_ASYNC | DRM_MODE_PAGE_FLIP_TARGET)

/*
 * Request a page flip on the specified crtc.
 *
 * This ioctl will ask KMS to schedule a page flip for the specified
 * crtc.  Once any pending rendering targeting the specified fb (as of
 * ioctl time) has completed, the crtc will be reprogrammed to display
 * that fb after the next vertical refresh.  The ioctl returns
 * immediately, but subsequent rendering to the current fb will block
 * in the execbuffer ioctl until the page flip happens.  If a page
 * flip is already pending as the ioctl is called, EBUSY will be
 * returned.
 *
 * Flag DRM_MODE_PAGE_FLIP_EVENT requests that drm sends back a vblank
 * event (see drm.h: struct drm_event_vblank) when the page flip is
 * done.  The user_data field passed in with this ioctl will be
 * returned as the user_data field in the vblank event struct.
 *
 * Flag DRM_MODE_PAGE_FLIP_ASYNC requests that the flip happen
 * 'as soon as possible', meaning that it not delay waiting for vblank.
 * This may cause tearing on the screen.
 *
 * The reserved field must be zero.
 */

struct drm_mode_crtc_page_flip {
    uint32_t crtc_id;
    uint32_t fb_id;
    uint32_t flags;
    uint32_t reserved;
    uint64_t user_data;
};

/*
 * Request a page flip on the specified crtc.
 *
 * Same as struct drm_mode_crtc_page_flip, but supports new flags and
 * re-purposes the reserved field:
 *
 * The sequence field must be zero unless either of the
 * DRM_MODE_PAGE_FLIP_TARGET_ABSOLUTE/RELATIVE flags is specified. When
 * the ABSOLUTE flag is specified, the sequence field denotes the absolute
 * vblank sequence when the flip should take effect. When the RELATIVE
 * flag is specified, the sequence field denotes the relative (to the
 * current one when the ioctl is called) vblank sequence when the flip
 * should take effect. NOTE: DRM_IOCTL_WAIT_VBLANK must still be used to
 * make sure the vblank sequence before the target one has passed before
 * calling this ioctl. The purpose of the
 * DRM_MODE_PAGE_FLIP_TARGET_ABSOLUTE/RELATIVE flags is merely to clarify
 * the target for when code dealing with a page flip runs during a
 * vertical blank period.
 */

struct drm_mode_crtc_page_flip_target {
    uint32_t crtc_id;
    uint32_t fb_id;
    uint32_t flags;
    uint32_t sequence;
    uint64_t user_data;
};

// create a dumb scanout buffer
struct drm_mode_create_dumb {
    uint32_t height;
    uint32_t width;
    uint32_t bpp;
    uint32_t flags;
    // handle, pitch, size will be returned
    uint32_t handle;
    uint32_t pitch;
    uint64_t size;
};

// set up for mmap of a dumb scanout buffer
struct drm_mode_map_dumb {
    // Handle for the object being mapped.
    uint32_t handle;
    uint32_t pad;
    /**
     * Fake offset to use for subsequent mmap call
     *
     * This is a fixed-size type for 32/64 compatibility.
     */
    uint64_t offset;
};

struct drm_mode_destroy_dumb {
    uint32_t handle;
};

// page-flip flags are valid, plus:
#define DRM_MODE_ATOMIC_TEST_ONLY 0x0100
#define DRM_MODE_ATOMIC_NONBLOCK  0x0200
#define DRM_MODE_ATOMIC_ALLOW_MODESET 0x0400

#define DRM_MODE_ATOMIC_FLAGS (DRM_MODE_PAGE_FLIP_EVENT | DRM_MODE_PAGE_FLIP_ASYNC | DRM_MODE_ATOMIC_TEST_ONLY | \
        DRM_MODE_ATOMIC_NONBLOCK | DRM_MODE_ATOMIC_ALLOW_MODESET)

struct drm_mode_atomic {
    uint32_t flags;
    uint32_t count_objs;
    uint64_t objs_ptr;
    uint64_t count_props_ptr;
    uint64_t props_ptr;
    uint64_t prop_values_ptr;
    uint64_t reserved;
    uint64_t user_data;
};

struct drm_format_modifier_blob {
#define FORMAT_BLOB_CURRENT 1
    // Version of this blob format
    uint32_t version;

    // Flags
    uint32_t flags;

    // Number of fourcc formats supported
    uint32_t count_formats;

    // Where in this blob the formats exist (in bytes)
    uint32_t formats_offset;

    // Number of drm_format_modifiers
    uint32_t count_modifiers;

    // Where in this blob the modifiers exist (in bytes)
    uint32_t modifiers_offset;

    // uint32_t formats[]
    // struct drm_format_modifier modifiers[]
};

struct drm_format_modifier {
    /* Bitmask of formats in get_plane format list this info applies to. The
     * offset allows a sliding window of which 64 formats (bits).
     *
     * Some examples:
     * In today's world with < 65 formats, and formats 0, and 2 are
     * supported
     * 0x0000000000000005
     *   ^-offset = 0, formats = 5
     *
     * If the number formats grew to 128, and formats 98-102 are
     * supported with the modifier:
     *
     * 0x0000007c00000000 0000000000000000
     *   ^
     *   |__offset = 64, formats = 0x7c00000000
     *
     */
    uint64_t formats;
    uint32_t offset;
    uint32_t pad;

    // The modifier that applies to the >get_plane format list bitmask.
    uint64_t modifier;
};

/**
 * struct drm_mode_create_blob - Create New blob property
 *
 * Create a new 'blob' data property, copying length bytes from data pointer,
 * and returning new blob ID.
 */
struct drm_mode_create_blob {
    /** @data: Pointer to data to copy. */
    uint64_t data;
    /** @length: Length of data to copy. */
    uint32_t length;
    /** @blob_id: Return: new property ID. */
    uint32_t blob_id;
};

/**
 * struct drm_mode_destroy_blob - Destroy user blob
 * @blob_id: blob_id to destroy
 *
 * Destroy a user-created blob property.
 *
 * User-space can release blobs as soon as they do not need to refer to them by
 * their blob object ID.  For instance, if you are using a MODE_ID blob in an
 * atomic commit and you will not make another commit re-using the same ID, you
 * can destroy the blob as soon as the commit has been issued, without waiting
 * for it to complete.
 */
struct drm_mode_destroy_blob {
    uint32_t blob_id;
};

/**
 * struct drm_mode_create_lease - Create lease
 *
 * Lease mode resources, creating another drm_master.
 */
struct drm_mode_create_lease {
    /** @object_ids: Pointer to array of object ids (uint32_t) */
    uint64_t object_ids;
    /** @object_count: Number of object ids */
    uint32_t object_count;
    /** @flags: flags for new FD (O_CLOEXEC, etc) */
    uint32_t flags;

    /** @lessee_id: Return: unique identifier for lessee. */
    uint32_t lessee_id;
    /** @fd: Return: file descriptor to new drm_master file */
    uint32_t fd;
};

/**
 * struct drm_mode_list_lessees - List lessees
 *
 * List lesses from a drm_master.
 */
struct drm_mode_list_lessees {
    /**
     * @count_lessees: Number of lessees.
     *
     * On input, provides length of the array.
     * On output, provides total number. No
     * more than the input number will be written
     * back, so two calls can be used to get
     * the size and then the data.
     */
    uint32_t count_lessees;
    /** @pad: Padding. */
    uint32_t pad;

    /**
     * @lessees_ptr: Pointer to lessees.
     *
     * Pointer to uint64_t array of lessee ids
     */
    uint64_t lessees_ptr;
};

/**
 * struct drm_mode_get_lease - Get Lease
 *
 * Get leased objects.
 */
struct drm_mode_get_lease {
    /**
     * @count_objects: Number of leased objects.
     *
     * On input, provides length of the array.
     * On output, provides total number. No
     * more than the input number will be written
     * back, so two calls can be used to get
     * the size and then the data.
     */
    uint32_t count_objects;
    /** @pad: Padding. */
    uint32_t pad;

    /**
     * @objects_ptr: Pointer to objects.
     *
     * Pointer to uint32_t array of object ids.
     */
    uint64_t objects_ptr;
};

/**
 * struct drm_mode_revoke_lease - Revoke lease
 */
struct drm_mode_revoke_lease {
    /** @lessee_id: Unique ID of lessee */
    uint32_t lessee_id;
};

/**
 * struct drm_mode_rect - Two dimensional rectangle.
 * @x1: Horizontal starting coordinate (inclusive).
 * @y1: Vertical starting coordinate (inclusive).
 * @x2: Horizontal ending coordinate (exclusive).
 * @y2: Vertical ending coordinate (exclusive).
 *
 * With drm subsystem using struct drm_rect to manage rectangular area this
 * export it to user-space.
 *
 * Currently used by drm_mode_atomic blob property FB_DAMAGE_CLIPS.
 */
struct drm_mode_rect {
    int32_t x1;
    int32_t y1;
    int32_t x2;
    int32_t y2;
};

// drm_connector.h
enum drm_connector_status {
    /**
     * @DRM_CONNECTOR_STATUS_CONNECTED: The connector is definitely connected to
     * a sink device, and can be enabled.
     */
    DRM_CONNECTOR_STATUS_CONNECTED = 1,
    /**
     * @DRM_CONNECTOR_STATUS_DISCONNECTED: The connector isn't connected to a
     * sink device which can be autodetect. For digital outputs like DP or
     * HDMI (which can be realiable probed) this means there's really
     * nothing there. It is driver-dependent whether a connector with this
     * status can be lit up or not.
     */
    DRM_CONNECTOR_STATUS_DISCONNECTED = 2,
    /**
     * @DRM_CONNECTOR_STATUS_UNKNOWN: The connector's status could not be
     * reliably detected. This happens when probing would either cause
     * flicker (like load-detection when the connector is in use), or when a
     * hardware resource isn't available (like when load-detection needs a
     * free CRTC). It should be possible to light up the connector with one
     * of the listed fallback modes. For default configuration userspace
     * should only try to light up connectors with unknown status when
     * there's not connector with @connector_status_connected.
     */
    DRM_CONNECTOR_STATUS_UNKNOWN = 3,
};

// prctl.h
#define ARCH_SET_GS 0x1001
#define ARCH_SET_FS 0x1002
#define ARCH_GET_FS 0x1003
#define ARCH_GET_GS 0x1004
#define ARCH_GET_CPUID 0x1011
#define ARCH_SET_CPUID 0x1012
