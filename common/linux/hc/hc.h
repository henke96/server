// Static assert some compiler assumptions.
_Static_assert(sizeof(long) == 8, "long not 8 bytes");
_Static_assert(-1 == ~0, "not two's complement");
_Static_assert((-1 >> 1) == -1, "not arithmetic shift right");

#if defined(__x86_64__)
#define hc_X86_64 1
#elif defined(__aarch64__)
#define hc_AARCH64 1
#endif

// ALIGN must be power of 2.
#define hc_ALIGN_FORWARD(X, ALIGN) (((X) + ((ALIGN) - 1)) & ~((ALIGN) - 1))
#define hc_UNREACHABLE __builtin_unreachable()
#define hc_UNUSED __attribute__((unused))
#define hc_PACKED __attribute__((packed))
#define hc_FALLTHROUGH __attribute__((fallthrough))
#define hc_ALIGNED(N) __attribute__((aligned(N)))
#define hc_ALWAYS_INLINE __attribute__((always_inline)) inline
#define hc_SECTION(NAME) __attribute__((section(NAME)))
#define hc_WEAK __attribute__((weak))
#define hc_ABS(N) __builtin_abs((N))
#define hc_ABS64(N) __builtin_labs((N))
#define hc_BSWAP16(N) __builtin_bswap16((N))
#define hc_BSWAP32(N) __builtin_bswap32((N))
#define hc_BSWAP64(N) __builtin_bswap64((N))
#define hc_MEMCPY(DEST, SRC, N) __builtin_memcpy((DEST), (SRC), (N))
#define hc_MEMMOVE(DEST, SRC, N) __builtin_memmove((DEST), (SRC), (N))
#define hc_MEMCMP(LEFT, RIGHT, N) __builtin_memcmp((LEFT), (RIGHT), (N))

// Atomics
#define hc_ATOMIC_RELAXED __ATOMIC_RELAXED
#define hc_ATOMIC_ACQUIRE __ATOMIC_ACQUIRE
#define hc_ATOMIC_RELEASE __ATOMIC_RELEASE
#define hc_ATOMIC_ACQ_REL __ATOMIC_ACQ_REL
#define hc_ATOMIC_SEQ_CST __ATOMIC_SEQ_CST

#define hc_ATOMIC_EXCHANGE(PTR, VAL, MEMORDER) __atomic_exchange_n((PTR), (VAL), (MEMORDER))
#define hc_ATOMIC_COMPARE_EXCHANGE(PTR, EXPECTED, DESIRED, SUCCESS_MEMORDER, FAILURE_MEMORDER) __atomic_compare_exchange_n((PTR), (EXPECTED), (DESIRED), 0, (SUCCESS_MEMORDER), (FAILURE_MEMORDER))
#define hc_ATOMIC_STORE(PTR, VAL, MEMORDER) __atomic_store_n((PTR), (VAL), (MEMORDER))
#define hc_ATOMIC_LOAD(PTR, MEMORDER) __atomic_load_n((PTR), (MEMORDER))

#if hc_X86_64
#define hc_ATOMIC_PAUSE __builtin_ia32_pause()
#elif hc_AARCH64
#define hc_ATOMIC_PAUSE asm volatile("yield")
#endif


// Syscalls
#if hc_X86_64
#define hc_SYSCALL0(NUM) \
register long ret asm("rax") = (NUM); \
asm volatile ( \
    "syscall\n" \
    : "+r"(ret) \
    : \
    : "rcx", "r11", "memory" \
);

#define hc_SYSCALL1(NUM, ARG1) \
register long ret asm("rax") = (NUM); \
register long arg1 asm("rdi") = (long)(ARG1); \
asm volatile ( \
    "syscall\n" \
    : "+r"(ret) \
    : "r"(arg1) \
    : "rcx", "r11", "memory" \
);

#define hc_SYSCALL2(NUM, ARG1, ARG2) \
register long ret asm("rax") = (NUM); \
register long arg1 asm("rdi") = (long)(ARG1); \
register long arg2 asm("rsi") = (long)(ARG2); \
asm volatile ( \
    "syscall\n" \
    : "+r"(ret) \
    : "r"(arg1), "r"(arg2) \
    : "rcx", "r11", "memory" \
);

#define hc_SYSCALL3(NUM, ARG1, ARG2, ARG3) \
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

#define hc_SYSCALL4(NUM, ARG1, ARG2, ARG3, ARG4) \
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

#define hc_SYSCALL5(NUM, ARG1, ARG2, ARG3, ARG4, ARG5) \
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

#define hc_SYSCALL6(NUM, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
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

#define hc_NR_read 0
#define hc_NR_write 1
#define hc_NR_open 2
#define hc_NR_close 3
#define hc_NR_stat 4
#define hc_NR_fstat 5
#define hc_NR_lstat 6
#define hc_NR_poll 7
#define hc_NR_lseek 8
#define hc_NR_mmap 9
#define hc_NR_mprotect 10
#define hc_NR_munmap 11
#define hc_NR_brk 12
#define hc_NR_rt_sigaction 13
#define hc_NR_rt_sigprocmask 14
#define hc_NR_rt_sigreturn 15
#define hc_NR_ioctl 16
#define hc_NR_pread64 17
#define hc_NR_pwrite64 18
#define hc_NR_readv 19
#define hc_NR_writev 20
#define hc_NR_access 21
#define hc_NR_pipe 22
#define hc_NR_select 23
#define hc_NR_sched_yield 24
#define hc_NR_mremap 25
#define hc_NR_msync 26
#define hc_NR_mincore 27
#define hc_NR_madvise 28
#define hc_NR_shmget 29
#define hc_NR_shmat 30
#define hc_NR_shmctl 31
#define hc_NR_dup 32
#define hc_NR_dup2 33
#define hc_NR_pause 34
#define hc_NR_nanosleep 35
#define hc_NR_getitimer 36
#define hc_NR_alarm 37
#define hc_NR_setitimer 38
#define hc_NR_getpid 39
#define hc_NR_sendfile 40
#define hc_NR_socket 41
#define hc_NR_connect 42
#define hc_NR_accept 43
#define hc_NR_sendto 44
#define hc_NR_recvfrom 45
#define hc_NR_sendmsg 46
#define hc_NR_recvmsg 47
#define hc_NR_shutdown 48
#define hc_NR_bind 49
#define hc_NR_listen 50
#define hc_NR_getsockname 51
#define hc_NR_getpeername 52
#define hc_NR_socketpair 53
#define hc_NR_setsockopt 54
#define hc_NR_getsockopt 55
#define hc_NR_clone 56
#define hc_NR_fork 57
#define hc_NR_vfork 58
#define hc_NR_execve 59
#define hc_NR_exit 60
#define hc_NR_wait4 61
#define hc_NR_kill 62
#define hc_NR_uname 63
#define hc_NR_semget 64
#define hc_NR_semop 65
#define hc_NR_semctl 66
#define hc_NR_shmdt 67
#define hc_NR_msgget 68
#define hc_NR_msgsnd 69
#define hc_NR_msgrcv 70
#define hc_NR_msgctl 71
#define hc_NR_fcntl 72
#define hc_NR_flock 73
#define hc_NR_fsync 74
#define hc_NR_fdatasync 75
#define hc_NR_truncate 76
#define hc_NR_ftruncate 77
#define hc_NR_getdents 78
#define hc_NR_getcwd 79
#define hc_NR_chdir 80
#define hc_NR_fchdir 81
#define hc_NR_rename 82
#define hc_NR_mkdir 83
#define hc_NR_rmdir 84
#define hc_NR_creat 85
#define hc_NR_link 86
#define hc_NR_unlink 87
#define hc_NR_symlink 88
#define hc_NR_readlink 89
#define hc_NR_chmod 90
#define hc_NR_fchmod 91
#define hc_NR_chown 92
#define hc_NR_fchown 93
#define hc_NR_lchown 94
#define hc_NR_umask 95
#define hc_NR_gettimeofday 96
#define hc_NR_getrlimit 97
#define hc_NR_getrusage 98
#define hc_NR_sysinfo 99
#define hc_NR_times 100
#define hc_NR_ptrace 101
#define hc_NR_getuid 102
#define hc_NR_syslog 103
#define hc_NR_getgid 104
#define hc_NR_setuid 105
#define hc_NR_setgid 106
#define hc_NR_geteuid 107
#define hc_NR_getegid 108
#define hc_NR_setpgid 109
#define hc_NR_getppid 110
#define hc_NR_getpgrp 111
#define hc_NR_setsid 112
#define hc_NR_setreuid 113
#define hc_NR_setregid 114
#define hc_NR_getgroups 115
#define hc_NR_setgroups 116
#define hc_NR_setresuid 117
#define hc_NR_getresuid 118
#define hc_NR_setresgid 119
#define hc_NR_getresgid 120
#define hc_NR_getpgid 121
#define hc_NR_setfsuid 122
#define hc_NR_setfsgid 123
#define hc_NR_getsid 124
#define hc_NR_capget 125
#define hc_NR_capset 126
#define hc_NR_rt_sigpending 127
#define hc_NR_rt_sigtimedwait 128
#define hc_NR_rt_sigqueueinfo 129
#define hc_NR_rt_sigsuspend 130
#define hc_NR_sigaltstack 131
#define hc_NR_utime 132
#define hc_NR_mknod 133
#define hc_NR_uselib 134
#define hc_NR_personality 135
#define hc_NR_ustat 136
#define hc_NR_statfs 137
#define hc_NR_fstatfs 138
#define hc_NR_sysfs 139
#define hc_NR_getpriority 140
#define hc_NR_setpriority 141
#define hc_NR_sched_setparam 142
#define hc_NR_sched_getparam 143
#define hc_NR_sched_setscheduler 144
#define hc_NR_sched_getscheduler 145
#define hc_NR_sched_get_priority_max 146
#define hc_NR_sched_get_priority_min 147
#define hc_NR_sched_rr_get_interval 148
#define hc_NR_mlock 149
#define hc_NR_munlock 150
#define hc_NR_mlockall 151
#define hc_NR_munlockall 152
#define hc_NR_vhangup 153
#define hc_NR_modify_ldt 154
#define hc_NR_pivot_root 155
#define hc_NR__sysctl 156
#define hc_NR_prctl 157
#define hc_NR_arch_prctl 158
#define hc_NR_adjtimex 159
#define hc_NR_setrlimit 160
#define hc_NR_chroot 161
#define hc_NR_sync 162
#define hc_NR_acct 163
#define hc_NR_settimeofday 164
#define hc_NR_mount 165
#define hc_NR_umount2 166
#define hc_NR_swapon 167
#define hc_NR_swapoff 168
#define hc_NR_reboot 169
#define hc_NR_sethostname 170
#define hc_NR_setdomainname 171
#define hc_NR_iopl 172
#define hc_NR_ioperm 173
#define hc_NR_create_module 174
#define hc_NR_init_module 175
#define hc_NR_delete_module 176
#define hc_NR_get_kernel_syms 177
#define hc_NR_query_module 178
#define hc_NR_quotactl 179
#define hc_NR_nfsservctl 180
#define hc_NR_getpmsg 181
#define hc_NR_putpmsg 182
#define hc_NR_afs_syscall 183
#define hc_NR_tuxcall 184
#define hc_NR_security 185
#define hc_NR_gettid 186
#define hc_NR_readahead 187
#define hc_NR_setxattr 188
#define hc_NR_lsetxattr 189
#define hc_NR_fsetxattr 190
#define hc_NR_getxattr 191
#define hc_NR_lgetxattr 192
#define hc_NR_fgetxattr 193
#define hc_NR_listxattr 194
#define hc_NR_llistxattr 195
#define hc_NR_flistxattr 196
#define hc_NR_removexattr 197
#define hc_NR_lremovexattr 198
#define hc_NR_fremovexattr 199
#define hc_NR_tkill 200
#define hc_NR_time 201
#define hc_NR_futex 202
#define hc_NR_sched_setaffinity 203
#define hc_NR_sched_getaffinity 204
#define hc_NR_set_thread_area 205
#define hc_NR_io_setup 206
#define hc_NR_io_destroy 207
#define hc_NR_io_getevents 208
#define hc_NR_io_submit 209
#define hc_NR_io_cancel 210
#define hc_NR_get_thread_area 211
#define hc_NR_lookup_dcookie 212
#define hc_NR_epoll_create 213
#define hc_NR_epoll_ctl_old 214
#define hc_NR_epoll_wait_old 215
#define hc_NR_remap_file_pages 216
#define hc_NR_getdents64 217
#define hc_NR_set_tid_address 218
#define hc_NR_restart_syscall 219
#define hc_NR_semtimedop 220
#define hc_NR_fadvise64 221
#define hc_NR_timer_create 222
#define hc_NR_timer_settime 223
#define hc_NR_timer_gettime 224
#define hc_NR_timer_getoverrun 225
#define hc_NR_timer_delete 226
#define hc_NR_clock_settime 227
#define hc_NR_clock_gettime 228
#define hc_NR_clock_getres 229
#define hc_NR_clock_nanosleep 230
#define hc_NR_exit_group 231
#define hc_NR_epoll_wait 232
#define hc_NR_epoll_ctl 233
#define hc_NR_tgkill 234
#define hc_NR_utimes 235
#define hc_NR_vserver 236
#define hc_NR_mbind 237
#define hc_NR_set_mempolicy 238
#define hc_NR_get_mempolicy 239
#define hc_NR_mq_open 240
#define hc_NR_mq_unlink 241
#define hc_NR_mq_timedsend 242
#define hc_NR_mq_timedreceive 243
#define hc_NR_mq_notify 244
#define hc_NR_mq_getsetattr 245
#define hc_NR_kexec_load 246
#define hc_NR_waitid 247
#define hc_NR_add_key 248
#define hc_NR_request_key 249
#define hc_NR_keyctl 250
#define hc_NR_ioprio_set 251
#define hc_NR_ioprio_get 252
#define hc_NR_inotify_init 253
#define hc_NR_inotify_add_watch 254
#define hc_NR_inotify_rm_watch 255
#define hc_NR_migrate_pages 256
#define hc_NR_openat 257
#define hc_NR_mkdirat 258
#define hc_NR_mknodat 259
#define hc_NR_fchownat 260
#define hc_NR_futimesat 261
#define hc_NR_newfstatat 262
#define hc_NR_unlinkat 263
#define hc_NR_renameat 264
#define hc_NR_linkat 265
#define hc_NR_symlinkat 266
#define hc_NR_readlinkat 267
#define hc_NR_fchmodat 268
#define hc_NR_faccessat 269
#define hc_NR_pselect6 270
#define hc_NR_ppoll 271
#define hc_NR_unshare 272
#define hc_NR_set_robust_list 273
#define hc_NR_get_robust_list 274
#define hc_NR_splice 275
#define hc_NR_tee 276
#define hc_NR_sync_file_range 277
#define hc_NR_vmsplice 278
#define hc_NR_move_pages 279
#define hc_NR_utimensat 280
#define hc_NR_epoll_pwait 281
#define hc_NR_signalfd 282
#define hc_NR_timerfd_create 283
#define hc_NR_eventfd 284
#define hc_NR_fallocate 285
#define hc_NR_timerfd_settime 286
#define hc_NR_timerfd_gettime 287
#define hc_NR_accept4 288
#define hc_NR_signalfd4 289
#define hc_NR_eventfd2 290
#define hc_NR_epoll_create1 291
#define hc_NR_dup3 292
#define hc_NR_pipe2 293
#define hc_NR_inotify_init1 294
#define hc_NR_preadv 295
#define hc_NR_pwritev 296
#define hc_NR_rt_tgsigqueueinfo 297
#define hc_NR_perf_event_open 298
#define hc_NR_recvmmsg 299
#define hc_NR_fanotify_init 300
#define hc_NR_fanotify_mark 301
#define hc_NR_prlimit64 302
#define hc_NR_name_to_handle_at 303
#define hc_NR_open_by_handle_at 304
#define hc_NR_clock_adjtime 305
#define hc_NR_syncfs 306
#define hc_NR_sendmmsg 307
#define hc_NR_setns 308
#define hc_NR_getcpu 309
#define hc_NR_process_vm_readv 310
#define hc_NR_process_vm_writev 311
#define hc_NR_kcmp 312
#define hc_NR_finit_module 313
#define hc_NR_sched_setattr 314
#define hc_NR_sched_getattr 315
#define hc_NR_renameat2 316
#define hc_NR_seccomp 317
#define hc_NR_getrandom 318
#define hc_NR_memfd_create 319
#define hc_NR_kexec_file_load 320
#define hc_NR_bpf 321
#define hc_NR_execveat 322
#define hc_NR_userfaultfd 323
#define hc_NR_membarrier 324
#define hc_NR_mlock2 325
#define hc_NR_copy_file_range 326
#define hc_NR_preadv2 327
#define hc_NR_pwritev2 328
#define hc_NR_pkey_mprotect 329
#define hc_NR_pkey_alloc 330
#define hc_NR_pkey_free 331
#define hc_NR_statx 332
#define hc_NR_io_pgetevents 333
#define hc_NR_rseq 334
#define hc_NR_pidfd_send_signal 424
#define hc_NR_io_uring_setup 425
#define hc_NR_io_uring_enter 426
#define hc_NR_io_uring_register 427
#define hc_NR_open_tree 428
#define hc_NR_move_mount 429
#define hc_NR_fsopen 430
#define hc_NR_fsconfig 431
#define hc_NR_fsmount 432
#define hc_NR_fspick 433
#define hc_NR_pidfd_open 434
#define hc_NR_clone3 435
#define hc_NR_close_range 436
#define hc_NR_openat2 437
#define hc_NR_pidfd_getfd 438
#define hc_NR_faccessat2 439
#define hc_NR_process_madvise 440
#define hc_NR_epoll_pwait2 441
#define hc_NR_mount_setattr 442
#define hc_NR_quotactl_fd 443
#define hc_NR_landlock_create_ruleset 444
#define hc_NR_landlock_add_rule 445
#define hc_NR_landlock_restrict_self 446
#define hc_NR_memfd_secret 447
#define hc_NR_process_mrelease 448

#elif hc_AARCH64
#define hc_SYSCALL0(NUM) \
register long num asm("x8") = (NUM); \
register long ret asm("x0"); \
asm volatile ( \
    "svc 0\n" \
    : "=r"(ret) \
    : "r"(num) \
    : "memory", "cc" \
);

#define hc_SYSCALL1(NUM, ARG1) \
register long num asm("x8") = (NUM); \
register long ret asm("x0") = (long)(ARG1); \
asm volatile ( \
    "svc 0\n" \
    : "+r"(ret) \
    : "r"(num) \
    : "memory", "cc" \
);

#define hc_SYSCALL2(NUM, ARG1, ARG2) \
register long num asm("x8") = (NUM); \
register long ret asm("x0") = (long)(ARG1); \
register long arg2 asm("x1") = (long)(ARG2); \
asm volatile ( \
    "svc 0\n" \
    : "+r"(ret) \
    : "r"(arg2), "r"(num) \
    : "memory", "cc" \
);

#define hc_SYSCALL3(NUM, ARG1, ARG2, ARG3) \
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

#define hc_SYSCALL4(NUM, ARG1, ARG2, ARG3, ARG4) \
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

#define hc_SYSCALL5(NUM, ARG1, ARG2, ARG3, ARG4, ARG5) \
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

#define hc_SYSCALL6(NUM, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
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

#define hc_NR_io_setup 0
#define hc_NR_io_destroy 1
#define hc_NR_io_submit 2
#define hc_NR_io_cancel 3
#define hc_NR_io_getevents 4
#define hc_NR_setxattr 5
#define hc_NR_lsetxattr 6
#define hc_NR_fsetxattr 7
#define hc_NR_getxattr 8
#define hc_NR_lgetxattr 9
#define hc_NR_fgetxattr 10
#define hc_NR_listxattr 11
#define hc_NR_llistxattr 12
#define hc_NR_flistxattr 13
#define hc_NR_removexattr 14
#define hc_NR_lremovexattr 15
#define hc_NR_fremovexattr 16
#define hc_NR_getcwd 17
#define hc_NR_lookup_dcookie 18
#define hc_NR_eventfd2 19
#define hc_NR_epoll_create1 20
#define hc_NR_epoll_ctl 21
#define hc_NR_epoll_pwait 22
#define hc_NR_dup 23
#define hc_NR_dup3 24
#define hc_NR_fcntl 25
#define hc_NR_inotify_init1 26
#define hc_NR_inotify_add_watch 27
#define hc_NR_inotify_rm_watch 28
#define hc_NR_ioctl 29
#define hc_NR_ioprio_set 30
#define hc_NR_ioprio_get 31
#define hc_NR_flock 32
#define hc_NR_mknodat 33
#define hc_NR_mkdirat 34
#define hc_NR_unlinkat 35
#define hc_NR_symlinkat 36
#define hc_NR_linkat 37
#define hc_NR_renameat 38
#define hc_NR_umount2 39
#define hc_NR_mount 40
#define hc_NR_pivot_root 41
#define hc_NR_nfsservctl 42
#define hc_NR_statfs 43
#define hc_NR_fstatfs 44
#define hc_NR_truncate 45
#define hc_NR_ftruncate 46
#define hc_NR_fallocate 47
#define hc_NR_faccessat 48
#define hc_NR_chdir 49
#define hc_NR_fchdir 50
#define hc_NR_chroot 51
#define hc_NR_fchmod 52
#define hc_NR_fchmodat 53
#define hc_NR_fchownat 54
#define hc_NR_fchown 55
#define hc_NR_openat 56
#define hc_NR_close 57
#define hc_NR_vhangup 58
#define hc_NR_pipe2 59
#define hc_NR_quotactl 60
#define hc_NR_getdents64 61
#define hc_NR_lseek 62
#define hc_NR_read 63
#define hc_NR_write 64
#define hc_NR_readv 65
#define hc_NR_writev 66
#define hc_NR_pread64 67
#define hc_NR_pwrite64 68
#define hc_NR_preadv 69
#define hc_NR_pwritev 70
#define hc_NR_sendfile 71
#define hc_NR_pselect6 72
#define hc_NR_ppoll 73
#define hc_NR_signalfd4 74
#define hc_NR_vmsplice 75
#define hc_NR_splice 76
#define hc_NR_tee 77
#define hc_NR_readlinkat 78
#define hc_NR_newfstatat 79
#define hc_NR_fstat 80
#define hc_NR_sync 81
#define hc_NR_fsync 82
#define hc_NR_fdatasync 83
#define hc_NR_sync_file_range 84
#define hc_NR_timerfd_create 85
#define hc_NR_timerfd_settime 86
#define hc_NR_timerfd_gettime 87
#define hc_NR_utimensat 88
#define hc_NR_acct 89
#define hc_NR_capget 90
#define hc_NR_capset 91
#define hc_NR_personality 92
#define hc_NR_exit 93
#define hc_NR_exit_group 94
#define hc_NR_waitid 95
#define hc_NR_set_tid_address 96
#define hc_NR_unshare 97
#define hc_NR_futex 98
#define hc_NR_set_robust_list 99
#define hc_NR_get_robust_list 100
#define hc_NR_nanosleep 101
#define hc_NR_getitimer 102
#define hc_NR_setitimer 103
#define hc_NR_kexec_load 104
#define hc_NR_init_module 105
#define hc_NR_delete_module 106
#define hc_NR_timer_create 107
#define hc_NR_timer_gettime 108
#define hc_NR_timer_getoverrun 109
#define hc_NR_timer_settime 110
#define hc_NR_timer_delete 111
#define hc_NR_clock_settime 112
#define hc_NR_clock_gettime 113
#define hc_NR_clock_getres 114
#define hc_NR_clock_nanosleep 115
#define hc_NR_syslog 116
#define hc_NR_ptrace 117
#define hc_NR_sched_setparam 118
#define hc_NR_sched_setscheduler 119
#define hc_NR_sched_getscheduler 120
#define hc_NR_sched_getparam 121
#define hc_NR_sched_setaffinity 122
#define hc_NR_sched_getaffinity 123
#define hc_NR_sched_yield 124
#define hc_NR_sched_get_priority_max 125
#define hc_NR_sched_get_priority_min 126
#define hc_NR_sched_rr_get_interval 127
#define hc_NR_restart_syscall 128
#define hc_NR_kill 129
#define hc_NR_tkill 130
#define hc_NR_tgkill 131
#define hc_NR_sigaltstack 132
#define hc_NR_rt_sigsuspend 133
#define hc_NR_rt_sigaction 134
#define hc_NR_rt_sigprocmask 135
#define hc_NR_rt_sigpending 136
#define hc_NR_rt_sigtimedwait 137
#define hc_NR_rt_sigqueueinfo 138
#define hc_NR_rt_sigreturn 139
#define hc_NR_setpriority 140
#define hc_NR_getpriority 141
#define hc_NR_reboot 142
#define hc_NR_setregid 143
#define hc_NR_setgid 144
#define hc_NR_setreuid 145
#define hc_NR_setuid 146
#define hc_NR_setresuid 147
#define hc_NR_getresuid 148
#define hc_NR_setresgid 149
#define hc_NR_getresgid 150
#define hc_NR_setfsuid 151
#define hc_NR_setfsgid 152
#define hc_NR_times 153
#define hc_NR_setpgid 154
#define hc_NR_getpgid 155
#define hc_NR_getsid 156
#define hc_NR_setsid 157
#define hc_NR_getgroups 158
#define hc_NR_setgroups 159
#define hc_NR_uname 160
#define hc_NR_sethostname 161
#define hc_NR_setdomainname 162
#define hc_NR_getrlimit 163
#define hc_NR_setrlimit 164
#define hc_NR_getrusage 165
#define hc_NR_umask 166
#define hc_NR_prctl 167
#define hc_NR_getcpu 168
#define hc_NR_gettimeofday 169
#define hc_NR_settimeofday 170
#define hc_NR_adjtimex 171
#define hc_NR_getpid 172
#define hc_NR_getppid 173
#define hc_NR_getuid 174
#define hc_NR_geteuid 175
#define hc_NR_getgid 176
#define hc_NR_getegid 177
#define hc_NR_gettid 178
#define hc_NR_sysinfo 179
#define hc_NR_mq_open 180
#define hc_NR_mq_unlink 181
#define hc_NR_mq_timedsend 182
#define hc_NR_mq_timedreceive 183
#define hc_NR_mq_notify 184
#define hc_NR_mq_getsetattr 185
#define hc_NR_msgget 186
#define hc_NR_msgctl 187
#define hc_NR_msgrcv 188
#define hc_NR_msgsnd 189
#define hc_NR_semget 190
#define hc_NR_semctl 191
#define hc_NR_semtimedop 192
#define hc_NR_semop 193
#define hc_NR_shmget 194
#define hc_NR_shmctl 195
#define hc_NR_shmat 196
#define hc_NR_shmdt 197
#define hc_NR_socket 198
#define hc_NR_socketpair 199
#define hc_NR_bind 200
#define hc_NR_listen 201
#define hc_NR_accept 202
#define hc_NR_connect 203
#define hc_NR_getsockname 204
#define hc_NR_getpeername 205
#define hc_NR_sendto 206
#define hc_NR_recvfrom 207
#define hc_NR_setsockopt 208
#define hc_NR_getsockopt 209
#define hc_NR_shutdown 210
#define hc_NR_sendmsg 211
#define hc_NR_recvmsg 212
#define hc_NR_readahead 213
#define hc_NR_brk 214
#define hc_NR_munmap 215
#define hc_NR_mremap 216
#define hc_NR_add_key 217
#define hc_NR_request_key 218
#define hc_NR_keyctl 219
#define hc_NR_clone 220
#define hc_NR_execve 221
#define hc_NR_mmap 222
#define hc_NR_fadvise64 223
#define hc_NR_swapon 224
#define hc_NR_swapoff 225
#define hc_NR_mprotect 226
#define hc_NR_msync 227
#define hc_NR_mlock 228
#define hc_NR_munlock 229
#define hc_NR_mlockall 230
#define hc_NR_munlockall 231
#define hc_NR_mincore 232
#define hc_NR_madvise 233
#define hc_NR_remap_file_pages 234
#define hc_NR_mbind 235
#define hc_NR_get_mempolicy 236
#define hc_NR_set_mempolicy 237
#define hc_NR_migrate_pages 238
#define hc_NR_move_pages 239
#define hc_NR_rt_tgsigqueueinfo 240
#define hc_NR_perf_event_open 241
#define hc_NR_accept4 242
#define hc_NR_recvmmsg 243
#define hc_NR_wait4 260
#define hc_NR_prlimit64 261
#define hc_NR_fanotify_init 262
#define hc_NR_fanotify_mark 263
#define hc_NR_name_to_handle_at 264
#define hc_NR_open_by_handle_at 265
#define hc_NR_clock_adjtime 266
#define hc_NR_syncfs 267
#define hc_NR_setns 268
#define hc_NR_sendmmsg 269
#define hc_NR_process_vm_readv 270
#define hc_NR_process_vm_writev 271
#define hc_NR_kcmp 272
#define hc_NR_finit_module 273
#define hc_NR_sched_setattr 274
#define hc_NR_sched_getattr 275
#define hc_NR_renameat2 276
#define hc_NR_seccomp 277
#define hc_NR_getrandom 278
#define hc_NR_memfd_create 279
#define hc_NR_bpf 280
#define hc_NR_execveat 281
#define hc_NR_userfaultfd 282
#define hc_NR_membarrier 283
#define hc_NR_mlock2 284
#define hc_NR_copy_file_range 285
#define hc_NR_preadv2 286
#define hc_NR_pwritev2 287
#define hc_NR_pkey_mprotect 288
#define hc_NR_pkey_alloc 289
#define hc_NR_pkey_free 290
#define hc_NR_statx 291
#define hc_NR_io_pgetevents 292
#define hc_NR_rseq 293
#define hc_NR_kexec_file_load 294
#define hc_NR_pidfd_send_signal 424
#define hc_NR_io_uring_setup 425
#define hc_NR_io_uring_enter 426
#define hc_NR_io_uring_register 427
#define hc_NR_open_tree 428
#define hc_NR_move_mount 429
#define hc_NR_fsopen 430
#define hc_NR_fsconfig 431
#define hc_NR_fsmount 432
#define hc_NR_fspick 433
#define hc_NR_pidfd_open 434
#define hc_NR_clone3 435
#define hc_NR_close_range 436
#define hc_NR_openat2 437
#define hc_NR_pidfd_getfd 438
#define hc_NR_faccessat2 439
#define hc_NR_process_madvise 440
#define hc_NR_epoll_pwait2 441
#define hc_NR_mount_setattr 442
#define hc_NR_quotactl_fd 443
#define hc_NR_landlock_create_ruleset 444
#define hc_NR_landlock_add_rule 445
#define hc_NR_landlock_restrict_self 446
#define hc_NR_memfd_secret 447
#define hc_NR_process_mrelease 448

#endif
