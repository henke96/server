// Common
#define NULL ((void *)0)

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned long long uint64_t;
typedef signed long long int64_t;

typedef _Bool bool;
#define false 0
#define true 1

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define EPERM   1  // Operation not permitted
#define ENOENT  2  // No such file or directory
#define ESRCH   3  // No such process
#define EINTR   4  // Interrupted system call
#define EIO     5  // I/O error
#define ENXIO   6  // No such device or address
#define E2BIG   7  // Argument list too long
#define ENOEXEC 8  // Exec format error
#define EBADF   9  // Bad file number
#define ECHILD  10 // No child processes
#define EAGAIN  11 // Try again
#define ENOMEM  12 // Out of memory
#define EACCES  13 // Permission denied
#define EFAULT  14 // Bad address
#define ENOTBLK 15 // Block device required
#define EBUSY   16 // Device or resource busy
#define EEXIST  17 // File exists
#define EXDEV   18 // Cross-device link
#define ENODEV  19 // No such device
#define ENOTDIR 20 // Not a directory
#define EISDIR  21 // Is a directory
#define EINVAL  22 // Invalid argument
#define ENFILE  23 // File table overflow
#define EMFILE  24 // Too many open files
#define ENOTTY  25 // Not a typewriter
#define ETXTBSY 26 // Text file busy
#define EFBIG   27 // File too large
#define ENOSPC  28 // No space left on device
#define ESPIPE  29 // Illegal seek
#define EROFS   30 // Read-only file system
#define EMLINK  31 // Too many links
#define EPIPE   32 // Broken pipe
#define EDOM    33 // Math argument out of domain of func
#define ERANGE  34 // Math result not representable

// Specific
#if defined(__x86_64__)
asm(
    ".section .text\n"
    ".global _start\n"
    "_start:\n"
    "pop %rdi\n"                // argc   (first arg, %rdi)
    "mov %rsp, %rsi\n"          // argv[] (second arg, %rsi)
    "lea 8(%rsi,%rdi,8),%rdx\n" // then a NULL then envp (third arg, %rdx)
    "and $-16, %rsp\n"          // x86 ABI : esp must be 16-byte aligned when
    "sub $8, %rsp\n"            // entering the callee
    "call main\n"               // main() returns the status code, we'll exit with it.
    "movzb %al, %rdi\n"         // retrieve exit code from 8 lower bits
    "mov $60, %rax\n"           // NR_exit == 60
    "syscall\n"                 // really exit
);

#define SYSCALL0(NUM) \
register long ret asm("rax") = (NUM); \
asm volatile ( \
    "syscall\n" \
    : "+r"(ret) \
    : \
    : "rcx", "r11", "memory" \
);

#define SYSCALL1(NUM, ARG1) \
register long ret asm("rax") = (NUM); \
register long arg1 asm("rdi") = (long)(ARG1); \
asm volatile ( \
    "syscall\n" \
    : "+r"(ret) \
    : "r"(arg1) \
    : "rcx", "r11", "memory" \
);

#define SYSCALL2(NUM, ARG1, ARG2) \
register long ret asm("rax") = (NUM); \
register long arg1 asm("rdi") = (long)(ARG1); \
register long arg2 asm("rsi") = (long)(ARG2); \
asm volatile ( \
    "syscall\n" \
    : "+r"(ret) \
    : "r"(arg1), "r"(arg2) \
    : "rcx", "r11", "memory" \
);

#define SYSCALL3(NUM, ARG1, ARG2, ARG3) \
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

#define SYSCALL4(NUM, ARG1, ARG2, ARG3, ARG4) \
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

#define SYSCALL5(NUM, ARG1, ARG2, ARG3, ARG4, ARG5) \
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

#define SYSCALL6(NUM, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
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

#define __NR_read 0
#define __NR_write 1
#define __NR_open 2
#define __NR_close 3
#define __NR_stat 4
#define __NR_fstat 5
#define __NR_lstat 6
#define __NR_poll 7
#define __NR_lseek 8
#define __NR_mmap 9
#define __NR_mprotect 10
#define __NR_munmap 11
#define __NR_brk 12
#define __NR_rt_sigaction 13
#define __NR_rt_sigprocmask 14
#define __NR_rt_sigreturn 15
#define __NR_ioctl 16
#define __NR_pread64 17
#define __NR_pwrite64 18
#define __NR_readv 19
#define __NR_writev 20
#define __NR_access 21
#define __NR_pipe 22
#define __NR_select 23
#define __NR_sched_yield 24
#define __NR_mremap 25
#define __NR_msync 26
#define __NR_mincore 27
#define __NR_madvise 28
#define __NR_shmget 29
#define __NR_shmat 30
#define __NR_shmctl 31
#define __NR_dup 32
#define __NR_dup2 33
#define __NR_pause 34
#define __NR_nanosleep 35
#define __NR_getitimer 36
#define __NR_alarm 37
#define __NR_setitimer 38
#define __NR_getpid 39
#define __NR_sendfile 40
#define __NR_socket 41
#define __NR_connect 42
#define __NR_accept 43
#define __NR_sendto 44
#define __NR_recvfrom 45
#define __NR_sendmsg 46
#define __NR_recvmsg 47
#define __NR_shutdown 48
#define __NR_bind 49
#define __NR_listen 50
#define __NR_getsockname 51
#define __NR_getpeername 52
#define __NR_socketpair 53
#define __NR_setsockopt 54
#define __NR_getsockopt 55
#define __NR_clone 56
#define __NR_fork 57
#define __NR_vfork 58
#define __NR_execve 59
#define __NR_exit 60
#define __NR_wait4 61
#define __NR_kill 62
#define __NR_uname 63
#define __NR_semget 64
#define __NR_semop 65
#define __NR_semctl 66
#define __NR_shmdt 67
#define __NR_msgget 68
#define __NR_msgsnd 69
#define __NR_msgrcv 70
#define __NR_msgctl 71
#define __NR_fcntl 72
#define __NR_flock 73
#define __NR_fsync 74
#define __NR_fdatasync 75
#define __NR_truncate 76
#define __NR_ftruncate 77
#define __NR_getdents 78
#define __NR_getcwd 79
#define __NR_chdir 80
#define __NR_fchdir 81
#define __NR_rename 82
#define __NR_mkdir 83
#define __NR_rmdir 84
#define __NR_creat 85
#define __NR_link 86
#define __NR_unlink 87
#define __NR_symlink 88
#define __NR_readlink 89
#define __NR_chmod 90
#define __NR_fchmod 91
#define __NR_chown 92
#define __NR_fchown 93
#define __NR_lchown 94
#define __NR_umask 95
#define __NR_gettimeofday 96
#define __NR_getrlimit 97
#define __NR_getrusage 98
#define __NR_sysinfo 99
#define __NR_times 100
#define __NR_ptrace 101
#define __NR_getuid 102
#define __NR_syslog 103
#define __NR_getgid 104
#define __NR_setuid 105
#define __NR_setgid 106
#define __NR_geteuid 107
#define __NR_getegid 108
#define __NR_setpgid 109
#define __NR_getppid 110
#define __NR_getpgrp 111
#define __NR_setsid 112
#define __NR_setreuid 113
#define __NR_setregid 114
#define __NR_getgroups 115
#define __NR_setgroups 116
#define __NR_setresuid 117
#define __NR_getresuid 118
#define __NR_setresgid 119
#define __NR_getresgid 120
#define __NR_getpgid 121
#define __NR_setfsuid 122
#define __NR_setfsgid 123
#define __NR_getsid 124
#define __NR_capget 125
#define __NR_capset 126
#define __NR_rt_sigpending 127
#define __NR_rt_sigtimedwait 128
#define __NR_rt_sigqueueinfo 129
#define __NR_rt_sigsuspend 130
#define __NR_sigaltstack 131
#define __NR_utime 132
#define __NR_mknod 133
#define __NR_uselib 134
#define __NR_personality 135
#define __NR_ustat 136
#define __NR_statfs 137
#define __NR_fstatfs 138
#define __NR_sysfs 139
#define __NR_getpriority 140
#define __NR_setpriority 141
#define __NR_sched_setparam 142
#define __NR_sched_getparam 143
#define __NR_sched_setscheduler 144
#define __NR_sched_getscheduler 145
#define __NR_sched_get_priority_max 146
#define __NR_sched_get_priority_min 147
#define __NR_sched_rr_get_interval 148
#define __NR_mlock 149
#define __NR_munlock 150
#define __NR_mlockall 151
#define __NR_munlockall 152
#define __NR_vhangup 153
#define __NR_modify_ldt 154
#define __NR_pivot_root 155
#define __NR__sysctl 156
#define __NR_prctl 157
#define __NR_arch_prctl 158
#define __NR_adjtimex 159
#define __NR_setrlimit 160
#define __NR_chroot 161
#define __NR_sync 162
#define __NR_acct 163
#define __NR_settimeofday 164
#define __NR_mount 165
#define __NR_umount2 166
#define __NR_swapon 167
#define __NR_swapoff 168
#define __NR_reboot 169
#define __NR_sethostname 170
#define __NR_setdomainname 171
#define __NR_iopl 172
#define __NR_ioperm 173
#define __NR_create_module 174
#define __NR_init_module 175
#define __NR_delete_module 176
#define __NR_get_kernel_syms 177
#define __NR_query_module 178
#define __NR_quotactl 179
#define __NR_nfsservctl 180
#define __NR_getpmsg 181
#define __NR_putpmsg 182
#define __NR_afs_syscall 183
#define __NR_tuxcall 184
#define __NR_security 185
#define __NR_gettid 186
#define __NR_readahead 187
#define __NR_setxattr 188
#define __NR_lsetxattr 189
#define __NR_fsetxattr 190
#define __NR_getxattr 191
#define __NR_lgetxattr 192
#define __NR_fgetxattr 193
#define __NR_listxattr 194
#define __NR_llistxattr 195
#define __NR_flistxattr 196
#define __NR_removexattr 197
#define __NR_lremovexattr 198
#define __NR_fremovexattr 199
#define __NR_tkill 200
#define __NR_time 201
#define __NR_futex 202
#define __NR_sched_setaffinity 203
#define __NR_sched_getaffinity 204
#define __NR_set_thread_area 205
#define __NR_io_setup 206
#define __NR_io_destroy 207
#define __NR_io_getevents 208
#define __NR_io_submit 209
#define __NR_io_cancel 210
#define __NR_get_thread_area 211
#define __NR_lookup_dcookie 212
#define __NR_epoll_create 213
#define __NR_epoll_ctl_old 214
#define __NR_epoll_wait_old 215
#define __NR_remap_file_pages 216
#define __NR_getdents64 217
#define __NR_set_tid_address 218
#define __NR_restart_syscall 219
#define __NR_semtimedop 220
#define __NR_fadvise64 221
#define __NR_timer_create 222
#define __NR_timer_settime 223
#define __NR_timer_gettime 224
#define __NR_timer_getoverrun 225
#define __NR_timer_delete 226
#define __NR_clock_settime 227
#define __NR_clock_gettime 228
#define __NR_clock_getres 229
#define __NR_clock_nanosleep 230
#define __NR_exit_group 231
#define __NR_epoll_wait 232
#define __NR_epoll_ctl 233
#define __NR_tgkill 234
#define __NR_utimes 235
#define __NR_vserver 236
#define __NR_mbind 237
#define __NR_set_mempolicy 238
#define __NR_get_mempolicy 239
#define __NR_mq_open 240
#define __NR_mq_unlink 241
#define __NR_mq_timedsend 242
#define __NR_mq_timedreceive 243
#define __NR_mq_notify 244
#define __NR_mq_getsetattr 245
#define __NR_kexec_load 246
#define __NR_waitid 247
#define __NR_add_key 248
#define __NR_request_key 249
#define __NR_keyctl 250
#define __NR_ioprio_set 251
#define __NR_ioprio_get 252
#define __NR_inotify_init 253
#define __NR_inotify_add_watch 254
#define __NR_inotify_rm_watch 255
#define __NR_migrate_pages 256
#define __NR_openat 257
#define __NR_mkdirat 258
#define __NR_mknodat 259
#define __NR_fchownat 260
#define __NR_futimesat 261
#define __NR_newfstatat 262
#define __NR_unlinkat 263
#define __NR_renameat 264
#define __NR_linkat 265
#define __NR_symlinkat 266
#define __NR_readlinkat 267
#define __NR_fchmodat 268
#define __NR_faccessat 269
#define __NR_pselect6 270
#define __NR_ppoll 271
#define __NR_unshare 272
#define __NR_set_robust_list 273
#define __NR_get_robust_list 274
#define __NR_splice 275
#define __NR_tee 276
#define __NR_sync_file_range 277
#define __NR_vmsplice 278
#define __NR_move_pages 279
#define __NR_utimensat 280
#define __NR_epoll_pwait 281
#define __NR_signalfd 282
#define __NR_timerfd_create 283
#define __NR_eventfd 284
#define __NR_fallocate 285
#define __NR_timerfd_settime 286
#define __NR_timerfd_gettime 287
#define __NR_accept4 288
#define __NR_signalfd4 289
#define __NR_eventfd2 290
#define __NR_epoll_create1 291
#define __NR_dup3 292
#define __NR_pipe2 293
#define __NR_inotify_init1 294
#define __NR_preadv 295
#define __NR_pwritev 296
#define __NR_rt_tgsigqueueinfo 297
#define __NR_perf_event_open 298
#define __NR_recvmmsg 299
#define __NR_fanotify_init 300
#define __NR_fanotify_mark 301
#define __NR_prlimit64 302
#define __NR_name_to_handle_at 303
#define __NR_open_by_handle_at 304
#define __NR_clock_adjtime 305
#define __NR_syncfs 306
#define __NR_sendmmsg 307
#define __NR_setns 308
#define __NR_getcpu 309
#define __NR_process_vm_readv 310
#define __NR_process_vm_writev 311
#define __NR_kcmp 312
#define __NR_finit_module 313
#define __NR_sched_setattr 314
#define __NR_sched_getattr 315
#define __NR_renameat2 316
#define __NR_seccomp 317
#define __NR_getrandom 318
#define __NR_memfd_create 319
#define __NR_kexec_file_load 320
#define __NR_bpf 321
#define __NR_execveat 322
#define __NR_userfaultfd 323
#define __NR_membarrier 324
#define __NR_mlock2 325
#define __NR_copy_file_range 326
#define __NR_preadv2 327
#define __NR_pwritev2 328
#define __NR_pkey_mprotect 329
#define __NR_pkey_alloc 330
#define __NR_pkey_free 331
#define __NR_statx 332
#define __NR_io_pgetevents 333
#define __NR_rseq 334
#define __NR_pidfd_send_signal 424
#define __NR_io_uring_setup 425
#define __NR_io_uring_enter 426
#define __NR_io_uring_register 427
#define __NR_open_tree 428
#define __NR_move_mount 429
#define __NR_fsopen 430
#define __NR_fsconfig 431
#define __NR_fsmount 432
#define __NR_fspick 433
#define __NR_pidfd_open 434
#define __NR_clone3 435
#define __NR_close_range 436
#define __NR_openat2 437
#define __NR_pidfd_getfd 438
#define __NR_faccessat2 439
#define __NR_process_madvise 440
#define __NR_epoll_pwait2 441
#define __NR_mount_setattr 442
#define __NR_quotactl_fd 443
#define __NR_landlock_create_ruleset 444
#define __NR_landlock_add_rule 445
#define __NR_landlock_restrict_self 446
#define __NR_memfd_secret 447
#define __NR_process_mrelease 448

#elif defined(__aarch64__)
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
    "and x0, x0, 0xff\n"          // limit exit code to 8 bits
    "mov x8, 93\n"                // NR_exit == 93
    "svc #0\n"
);

#define SYSCALL0(NUM) \
register long num asm("x8") = (NUM); \
register long ret asm("x0"); \
asm volatile ( \
    "svc 0\n" \
    : "=r"(ret) \
    : "r"(num) \
    : "memory", "cc" \
);

#define SYSCALL1(NUM, ARG1) \
register long num asm("x8") = (NUM); \
register long ret asm("x0") = (long)(ARG1); \
asm volatile ( \
    "svc 0\n" \
    : "+r"(ret) \
    : "r"(num) \
    : "memory", "cc" \
);

#define SYSCALL2(NUM, ARG1, ARG2) \
register long num asm("x8") = (NUM); \
register long ret asm("x0") = (long)(ARG1); \
register long arg2 asm("x1") = (long)(ARG2); \
asm volatile ( \
    "svc 0\n" \
    : "+r"(ret) \
    : "r"(arg2), "r"(num) \
    : "memory", "cc" \
);

#define SYSCALL3(NUM, ARG1, ARG2, ARG3) \
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

#define SYSCALL4(NUM, ARG1, ARG2, ARG3, ARG4) \
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

#define SYSCALL5(NUM, ARG1, ARG2, ARG3, ARG4, ARG5) \
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

#define SYSCALL6(NUM, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
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

#define __NR_io_setup 0
#define __NR_io_destroy 1
#define __NR_io_submit 2
#define __NR_io_cancel 3
#define __NR_io_getevents 4
#define __NR_setxattr 5
#define __NR_lsetxattr 6
#define __NR_fsetxattr 7
#define __NR_getxattr 8
#define __NR_lgetxattr 9
#define __NR_fgetxattr 10
#define __NR_listxattr 11
#define __NR_llistxattr 12
#define __NR_flistxattr 13
#define __NR_removexattr 14
#define __NR_lremovexattr 15
#define __NR_fremovexattr 16
#define __NR_getcwd 17
#define __NR_lookup_dcookie 18
#define __NR_eventfd2 19
#define __NR_epoll_create1 20
#define __NR_epoll_ctl 21
#define __NR_epoll_pwait 22
#define __NR_dup 23
#define __NR_dup3 24
#define __NR_fcntl 25
#define __NR_inotify_init1 26
#define __NR_inotify_add_watch 27
#define __NR_inotify_rm_watch 28
#define __NR_ioctl 29
#define __NR_ioprio_set 30
#define __NR_ioprio_get 31
#define __NR_flock 32
#define __NR_mknodat 33
#define __NR_mkdirat 34
#define __NR_unlinkat 35
#define __NR_symlinkat 36
#define __NR_linkat 37
#define __NR_renameat 38
#define __NR_umount2 39
#define __NR_mount 40
#define __NR_pivot_root 41
#define __NR_nfsservctl 42
#define __NR_statfs 43
#define __NR_fstatfs 44
#define __NR_truncate 45
#define __NR_ftruncate 46
#define __NR_fallocate 47
#define __NR_faccessat 48
#define __NR_chdir 49
#define __NR_fchdir 50
#define __NR_chroot 51
#define __NR_fchmod 52
#define __NR_fchmodat 53
#define __NR_fchownat 54
#define __NR_fchown 55
#define __NR_openat 56
#define __NR_close 57
#define __NR_vhangup 58
#define __NR_pipe2 59
#define __NR_quotactl 60
#define __NR_getdents64 61
#define __NR_lseek 62
#define __NR_read 63
#define __NR_write 64
#define __NR_readv 65
#define __NR_writev 66
#define __NR_pread64 67
#define __NR_pwrite64 68
#define __NR_preadv 69
#define __NR_pwritev 70
#define __NR_sendfile 71
#define __NR_pselect6 72
#define __NR_ppoll 73
#define __NR_signalfd4 74
#define __NR_vmsplice 75
#define __NR_splice 76
#define __NR_tee 77
#define __NR_readlinkat 78
#define __NR_newfstatat 79
#define __NR_fstat 80
#define __NR_sync 81
#define __NR_fsync 82
#define __NR_fdatasync 83
#define __NR_sync_file_range 84
#define __NR_timerfd_create 85
#define __NR_timerfd_settime 86
#define __NR_timerfd_gettime 87
#define __NR_utimensat 88
#define __NR_acct 89
#define __NR_capget 90
#define __NR_capset 91
#define __NR_personality 92
#define __NR_exit 93
#define __NR_exit_group 94
#define __NR_waitid 95
#define __NR_set_tid_address 96
#define __NR_unshare 97
#define __NR_futex 98
#define __NR_set_robust_list 99
#define __NR_get_robust_list 100
#define __NR_nanosleep 101
#define __NR_getitimer 102
#define __NR_setitimer 103
#define __NR_kexec_load 104
#define __NR_init_module 105
#define __NR_delete_module 106
#define __NR_timer_create 107
#define __NR_timer_gettime 108
#define __NR_timer_getoverrun 109
#define __NR_timer_settime 110
#define __NR_timer_delete 111
#define __NR_clock_settime 112
#define __NR_clock_gettime 113
#define __NR_clock_getres 114
#define __NR_clock_nanosleep 115
#define __NR_syslog 116
#define __NR_ptrace 117
#define __NR_sched_setparam 118
#define __NR_sched_setscheduler 119
#define __NR_sched_getscheduler 120
#define __NR_sched_getparam 121
#define __NR_sched_setaffinity 122
#define __NR_sched_getaffinity 123
#define __NR_sched_yield 124
#define __NR_sched_get_priority_max 125
#define __NR_sched_get_priority_min 126
#define __NR_sched_rr_get_interval 127
#define __NR_restart_syscall 128
#define __NR_kill 129
#define __NR_tkill 130
#define __NR_tgkill 131
#define __NR_sigaltstack 132
#define __NR_rt_sigsuspend 133
#define __NR_rt_sigaction 134
#define __NR_rt_sigprocmask 135
#define __NR_rt_sigpending 136
#define __NR_rt_sigtimedwait 137
#define __NR_rt_sigqueueinfo 138
#define __NR_rt_sigreturn 139
#define __NR_setpriority 140
#define __NR_getpriority 141
#define __NR_reboot 142
#define __NR_setregid 143
#define __NR_setgid 144
#define __NR_setreuid 145
#define __NR_setuid 146
#define __NR_setresuid 147
#define __NR_getresuid 148
#define __NR_setresgid 149
#define __NR_getresgid 150
#define __NR_setfsuid 151
#define __NR_setfsgid 152
#define __NR_times 153
#define __NR_setpgid 154
#define __NR_getpgid 155
#define __NR_getsid 156
#define __NR_setsid 157
#define __NR_getgroups 158
#define __NR_setgroups 159
#define __NR_uname 160
#define __NR_sethostname 161
#define __NR_setdomainname 162
#define __NR_getrlimit 163
#define __NR_setrlimit 164
#define __NR_getrusage 165
#define __NR_umask 166
#define __NR_prctl 167
#define __NR_getcpu 168
#define __NR_gettimeofday 169
#define __NR_settimeofday 170
#define __NR_adjtimex 171
#define __NR_getpid 172
#define __NR_getppid 173
#define __NR_getuid 174
#define __NR_geteuid 175
#define __NR_getgid 176
#define __NR_getegid 177
#define __NR_gettid 178
#define __NR_sysinfo 179
#define __NR_mq_open 180
#define __NR_mq_unlink 181
#define __NR_mq_timedsend 182
#define __NR_mq_timedreceive 183
#define __NR_mq_notify 184
#define __NR_mq_getsetattr 185
#define __NR_msgget 186
#define __NR_msgctl 187
#define __NR_msgrcv 188
#define __NR_msgsnd 189
#define __NR_semget 190
#define __NR_semctl 191
#define __NR_semtimedop 192
#define __NR_semop 193
#define __NR_shmget 194
#define __NR_shmctl 195
#define __NR_shmat 196
#define __NR_shmdt 197
#define __NR_socket 198
#define __NR_socketpair 199
#define __NR_bind 200
#define __NR_listen 201
#define __NR_accept 202
#define __NR_connect 203
#define __NR_getsockname 204
#define __NR_getpeername 205
#define __NR_sendto 206
#define __NR_recvfrom 207
#define __NR_setsockopt 208
#define __NR_getsockopt 209
#define __NR_shutdown 210
#define __NR_sendmsg 211
#define __NR_recvmsg 212
#define __NR_readahead 213
#define __NR_brk 214
#define __NR_munmap 215
#define __NR_mremap 216
#define __NR_add_key 217
#define __NR_request_key 218
#define __NR_keyctl 219
#define __NR_clone 220
#define __NR_execve 221
#define __NR_mmap 222
#define __NR_fadvise64 223
#define __NR_swapon 224
#define __NR_swapoff 225
#define __NR_mprotect 226
#define __NR_msync 227
#define __NR_mlock 228
#define __NR_munlock 229
#define __NR_mlockall 230
#define __NR_munlockall 231
#define __NR_mincore 232
#define __NR_madvise 233
#define __NR_remap_file_pages 234
#define __NR_mbind 235
#define __NR_get_mempolicy 236
#define __NR_set_mempolicy 237
#define __NR_migrate_pages 238
#define __NR_move_pages 239
#define __NR_rt_tgsigqueueinfo 240
#define __NR_perf_event_open 241
#define __NR_accept4 242
#define __NR_recvmmsg 243
#define __NR_wait4 260
#define __NR_prlimit64 261
#define __NR_fanotify_init 262
#define __NR_fanotify_mark 263
#define __NR_name_to_handle_at 264
#define __NR_open_by_handle_at 265
#define __NR_clock_adjtime 266
#define __NR_syncfs 267
#define __NR_setns 268
#define __NR_sendmmsg 269
#define __NR_process_vm_readv 270
#define __NR_process_vm_writev 271
#define __NR_kcmp 272
#define __NR_finit_module 273
#define __NR_sched_setattr 274
#define __NR_sched_getattr 275
#define __NR_renameat2 276
#define __NR_seccomp 277
#define __NR_getrandom 278
#define __NR_memfd_create 279
#define __NR_bpf 280
#define __NR_execveat 281
#define __NR_userfaultfd 282
#define __NR_membarrier 283
#define __NR_mlock2 284
#define __NR_copy_file_range 285
#define __NR_preadv2 286
#define __NR_pwritev2 287
#define __NR_pkey_mprotect 288
#define __NR_pkey_alloc 289
#define __NR_pkey_free 290
#define __NR_statx 291
#define __NR_io_pgetevents 292
#define __NR_rseq 293
#define __NR_kexec_file_load 294
#define __NR_pidfd_send_signal 424
#define __NR_io_uring_setup 425
#define __NR_io_uring_enter 426
#define __NR_io_uring_register 427
#define __NR_open_tree 428
#define __NR_move_mount 429
#define __NR_fsopen 430
#define __NR_fsconfig 431
#define __NR_fsmount 432
#define __NR_fspick 433
#define __NR_pidfd_open 434
#define __NR_clone3 435
#define __NR_close_range 436
#define __NR_openat2 437
#define __NR_pidfd_getfd 438
#define __NR_faccessat2 439
#define __NR_process_madvise 440
#define __NR_epoll_pwait2 441
#define __NR_mount_setattr 442
#define __NR_quotactl_fd 443
#define __NR_landlock_create_ruleset 444
#define __NR_landlock_add_rule 445
#define __NR_landlock_restrict_self 446
#define __NR_memfd_secret 447
#define __NR_process_mrelease 448

#else
#error "Unsupported architecture"
#endif

// Syscall wrappers
static inline int64_t write(int fd, const char *buf, uint64_t count) {
    SYSCALL3(__NR_write, fd, buf, count);
    return ret;
}