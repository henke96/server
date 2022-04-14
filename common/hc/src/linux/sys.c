#if hc_X86_64
#define sys_SYSCALL0(NUM) \
register int64_t ret asm("rax") = (NUM); \
asm volatile( \
    "syscall\n" \
    : "+r"(ret) \
    : \
    : "rcx", "r11", "memory" \
);

#define sys_SYSCALL1(NUM, ARG1) \
register int64_t ret asm("rax") = (NUM); \
register int64_t arg1 asm("rdi") = (int64_t)(ARG1); \
asm volatile( \
    "syscall\n" \
    : "+r"(ret) \
    : "r"(arg1) \
    : "rcx", "r11", "memory" \
);

#define sys_SYSCALL2(NUM, ARG1, ARG2) \
register int64_t ret asm("rax") = (NUM); \
register int64_t arg1 asm("rdi") = (int64_t)(ARG1); \
register int64_t arg2 asm("rsi") = (int64_t)(ARG2); \
asm volatile( \
    "syscall\n" \
    : "+r"(ret) \
    : "r"(arg1), "r"(arg2) \
    : "rcx", "r11", "memory" \
);

#define sys_SYSCALL3(NUM, ARG1, ARG2, ARG3) \
register int64_t ret asm("rax") = (NUM); \
register int64_t arg1 asm("rdi") = (int64_t)(ARG1); \
register int64_t arg2 asm("rsi") = (int64_t)(ARG2); \
register int64_t arg3 asm("rdx") = (int64_t)(ARG3); \
asm volatile( \
    "syscall\n" \
    : "+r"(ret) \
    : "r"(arg1), "r"(arg2), "r"(arg3) \
    : "rcx", "r11", "memory" \
);

#define sys_SYSCALL4(NUM, ARG1, ARG2, ARG3, ARG4) \
register int64_t ret asm("rax") = (NUM); \
register int64_t arg1 asm("rdi") = (int64_t)(ARG1); \
register int64_t arg2 asm("rsi") = (int64_t)(ARG2); \
register int64_t arg3 asm("rdx") = (int64_t)(ARG3); \
register int64_t arg4 asm("r10") = (int64_t)(ARG4); \
asm volatile( \
    "syscall\n" \
    : "+r"(ret) \
    : "r"(arg1), "r"(arg2), "r"(arg3), "r"(arg4) \
    : "rcx", "r11", "memory" \
);

#define sys_SYSCALL5(NUM, ARG1, ARG2, ARG3, ARG4, ARG5) \
register int64_t ret asm("rax") = (NUM); \
register int64_t arg1 asm("rdi") = (int64_t)(ARG1); \
register int64_t arg2 asm("rsi") = (int64_t)(ARG2); \
register int64_t arg3 asm("rdx") = (int64_t)(ARG3); \
register int64_t arg4 asm("r10") = (int64_t)(ARG4); \
register int64_t arg5 asm("r8") = (int64_t)(ARG5); \
asm volatile( \
    "syscall\n" \
    : "+r"(ret) \
    : "r"(arg1), "r"(arg2), "r"(arg3), "r"(arg4), "r"(arg5) \
    : "rcx", "r11", "memory" \
);

#define sys_SYSCALL6(NUM, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
register int64_t ret asm("rax") = (NUM); \
register int64_t arg1 asm("rdi") = (int64_t)(ARG1); \
register int64_t arg2 asm("rsi") = (int64_t)(ARG2); \
register int64_t arg3 asm("rdx") = (int64_t)(ARG3); \
register int64_t arg4 asm("r10") = (int64_t)(ARG4); \
register int64_t arg5 asm("r8") = (int64_t)(ARG5); \
register int64_t arg6 asm("r9") = (int64_t)(ARG6); \
asm volatile( \
    "syscall\n" \
    : "+r"(ret) \
    : "r"(arg1), "r"(arg2), "r"(arg3), "r"(arg4), "r"(arg5), "r"(arg6) \
    : "rcx", "r11", "memory" \
);

#define sys_NR_read 0
#define sys_NR_write 1
#define sys_NR_open 2
#define sys_NR_close 3
#define sys_NR_stat 4
#define sys_NR_fstat 5
#define sys_NR_lstat 6
#define sys_NR_poll 7
#define sys_NR_lseek 8
#define sys_NR_mmap 9
#define sys_NR_mprotect 10
#define sys_NR_munmap 11
#define sys_NR_brk 12
#define sys_NR_rt_sigaction 13
#define sys_NR_rt_sigprocmask 14
#define sys_NR_rt_sigreturn 15
#define sys_NR_ioctl 16
#define sys_NR_pread64 17
#define sys_NR_pwrite64 18
#define sys_NR_readv 19
#define sys_NR_writev 20
#define sys_NR_access 21
#define sys_NR_pipe 22
#define sys_NR_select 23
#define sys_NR_sched_yield 24
#define sys_NR_mremap 25
#define sys_NR_msync 26
#define sys_NR_mincore 27
#define sys_NR_madvise 28
#define sys_NR_shmget 29
#define sys_NR_shmat 30
#define sys_NR_shmctl 31
#define sys_NR_dup 32
#define sys_NR_dup2 33
#define sys_NR_pause 34
#define sys_NR_nanosleep 35
#define sys_NR_getitimer 36
#define sys_NR_alarm 37
#define sys_NR_setitimer 38
#define sys_NR_getpid 39
#define sys_NR_sendfile 40
#define sys_NR_socket 41
#define sys_NR_connect 42
#define sys_NR_accept 43
#define sys_NR_sendto 44
#define sys_NR_recvfrom 45
#define sys_NR_sendmsg 46
#define sys_NR_recvmsg 47
#define sys_NR_shutdown 48
#define sys_NR_bind 49
#define sys_NR_listen 50
#define sys_NR_getsockname 51
#define sys_NR_getpeername 52
#define sys_NR_socketpair 53
#define sys_NR_setsockopt 54
#define sys_NR_getsockopt 55
#define sys_NR_clone 56
#define sys_NR_fork 57
#define sys_NR_vfork 58
#define sys_NR_execve 59
#define sys_NR_exit 60
#define sys_NR_wait4 61
#define sys_NR_kill 62
#define sys_NR_uname 63
#define sys_NR_semget 64
#define sys_NR_semop 65
#define sys_NR_semctl 66
#define sys_NR_shmdt 67
#define sys_NR_msgget 68
#define sys_NR_msgsnd 69
#define sys_NR_msgrcv 70
#define sys_NR_msgctl 71
#define sys_NR_fcntl 72
#define sys_NR_flock 73
#define sys_NR_fsync 74
#define sys_NR_fdatasync 75
#define sys_NR_truncate 76
#define sys_NR_ftruncate 77
#define sys_NR_getdents 78
#define sys_NR_getcwd 79
#define sys_NR_chdir 80
#define sys_NR_fchdir 81
#define sys_NR_rename 82
#define sys_NR_mkdir 83
#define sys_NR_rmdir 84
#define sys_NR_creat 85
#define sys_NR_link 86
#define sys_NR_unlink 87
#define sys_NR_symlink 88
#define sys_NR_readlink 89
#define sys_NR_chmod 90
#define sys_NR_fchmod 91
#define sys_NR_chown 92
#define sys_NR_fchown 93
#define sys_NR_lchown 94
#define sys_NR_umask 95
#define sys_NR_gettimeofday 96
#define sys_NR_getrlimit 97
#define sys_NR_getrusage 98
#define sys_NR_sysinfo 99
#define sys_NR_times 100
#define sys_NR_ptrace 101
#define sys_NR_getuid 102
#define sys_NR_syslog 103
#define sys_NR_getgid 104
#define sys_NR_setuid 105
#define sys_NR_setgid 106
#define sys_NR_geteuid 107
#define sys_NR_getegid 108
#define sys_NR_setpgid 109
#define sys_NR_getppid 110
#define sys_NR_getpgrp 111
#define sys_NR_setsid 112
#define sys_NR_setreuid 113
#define sys_NR_setregid 114
#define sys_NR_getgroups 115
#define sys_NR_setgroups 116
#define sys_NR_setresuid 117
#define sys_NR_getresuid 118
#define sys_NR_setresgid 119
#define sys_NR_getresgid 120
#define sys_NR_getpgid 121
#define sys_NR_setfsuid 122
#define sys_NR_setfsgid 123
#define sys_NR_getsid 124
#define sys_NR_capget 125
#define sys_NR_capset 126
#define sys_NR_rt_sigpending 127
#define sys_NR_rt_sigtimedwait 128
#define sys_NR_rt_sigqueueinfo 129
#define sys_NR_rt_sigsuspend 130
#define sys_NR_sigaltstack 131
#define sys_NR_utime 132
#define sys_NR_mknod 133
#define sys_NR_uselib 134
#define sys_NR_personality 135
#define sys_NR_ustat 136
#define sys_NR_statfs 137
#define sys_NR_fstatfs 138
#define sys_NR_sysfs 139
#define sys_NR_getpriority 140
#define sys_NR_setpriority 141
#define sys_NR_sched_setparam 142
#define sys_NR_sched_getparam 143
#define sys_NR_sched_setscheduler 144
#define sys_NR_sched_getscheduler 145
#define sys_NR_sched_get_priority_max 146
#define sys_NR_sched_get_priority_min 147
#define sys_NR_sched_rr_get_interval 148
#define sys_NR_mlock 149
#define sys_NR_munlock 150
#define sys_NR_mlockall 151
#define sys_NR_munlockall 152
#define sys_NR_vhangup 153
#define sys_NR_modify_ldt 154
#define sys_NR_pivot_root 155
#define sys_NR__sysctl 156
#define sys_NR_prctl 157
#define sys_NR_arch_prctl 158
#define sys_NR_adjtimex 159
#define sys_NR_setrlimit 160
#define sys_NR_chroot 161
#define sys_NR_sync 162
#define sys_NR_acct 163
#define sys_NR_settimeofday 164
#define sys_NR_mount 165
#define sys_NR_umount2 166
#define sys_NR_swapon 167
#define sys_NR_swapoff 168
#define sys_NR_reboot 169
#define sys_NR_sethostname 170
#define sys_NR_setdomainname 171
#define sys_NR_iopl 172
#define sys_NR_ioperm 173
#define sys_NR_create_module 174
#define sys_NR_init_module 175
#define sys_NR_delete_module 176
#define sys_NR_get_kernel_syms 177
#define sys_NR_query_module 178
#define sys_NR_quotactl 179
#define sys_NR_nfsservctl 180
#define sys_NR_getpmsg 181
#define sys_NR_putpmsg 182
#define sys_NR_afs_syscall 183
#define sys_NR_tuxcall 184
#define sys_NR_security 185
#define sys_NR_gettid 186
#define sys_NR_readahead 187
#define sys_NR_setxattr 188
#define sys_NR_lsetxattr 189
#define sys_NR_fsetxattr 190
#define sys_NR_getxattr 191
#define sys_NR_lgetxattr 192
#define sys_NR_fgetxattr 193
#define sys_NR_listxattr 194
#define sys_NR_llistxattr 195
#define sys_NR_flistxattr 196
#define sys_NR_removexattr 197
#define sys_NR_lremovexattr 198
#define sys_NR_fremovexattr 199
#define sys_NR_tkill 200
#define sys_NR_time 201
#define sys_NR_futex 202
#define sys_NR_sched_setaffinity 203
#define sys_NR_sched_getaffinity 204
#define sys_NR_set_thread_area 205
#define sys_NR_io_setup 206
#define sys_NR_io_destroy 207
#define sys_NR_io_getevents 208
#define sys_NR_io_submit 209
#define sys_NR_io_cancel 210
#define sys_NR_get_thread_area 211
#define sys_NR_lookup_dcookie 212
#define sys_NR_epoll_create 213
#define sys_NR_epoll_ctl_old 214
#define sys_NR_epoll_wait_old 215
#define sys_NR_remap_file_pages 216
#define sys_NR_getdents64 217
#define sys_NR_set_tid_address 218
#define sys_NR_restart_syscall 219
#define sys_NR_semtimedop 220
#define sys_NR_fadvise64 221
#define sys_NR_timer_create 222
#define sys_NR_timer_settime 223
#define sys_NR_timer_gettime 224
#define sys_NR_timer_getoverrun 225
#define sys_NR_timer_delete 226
#define sys_NR_clock_settime 227
#define sys_NR_clock_gettime 228
#define sys_NR_clock_getres 229
#define sys_NR_clock_nanosleep 230
#define sys_NR_exit_group 231
#define sys_NR_epoll_wait 232
#define sys_NR_epoll_ctl 233
#define sys_NR_tgkill 234
#define sys_NR_utimes 235
#define sys_NR_vserver 236
#define sys_NR_mbind 237
#define sys_NR_set_mempolicy 238
#define sys_NR_get_mempolicy 239
#define sys_NR_mq_open 240
#define sys_NR_mq_unlink 241
#define sys_NR_mq_timedsend 242
#define sys_NR_mq_timedreceive 243
#define sys_NR_mq_notify 244
#define sys_NR_mq_getsetattr 245
#define sys_NR_kexec_load 246
#define sys_NR_waitid 247
#define sys_NR_add_key 248
#define sys_NR_request_key 249
#define sys_NR_keyctl 250
#define sys_NR_ioprio_set 251
#define sys_NR_ioprio_get 252
#define sys_NR_inotify_init 253
#define sys_NR_inotify_add_watch 254
#define sys_NR_inotify_rm_watch 255
#define sys_NR_migrate_pages 256
#define sys_NR_openat 257
#define sys_NR_mkdirat 258
#define sys_NR_mknodat 259
#define sys_NR_fchownat 260
#define sys_NR_futimesat 261
#define sys_NR_newfstatat 262
#define sys_NR_unlinkat 263
#define sys_NR_renameat 264
#define sys_NR_linkat 265
#define sys_NR_symlinkat 266
#define sys_NR_readlinkat 267
#define sys_NR_fchmodat 268
#define sys_NR_faccessat 269
#define sys_NR_pselect6 270
#define sys_NR_ppoll 271
#define sys_NR_unshare 272
#define sys_NR_set_robust_list 273
#define sys_NR_get_robust_list 274
#define sys_NR_splice 275
#define sys_NR_tee 276
#define sys_NR_sync_file_range 277
#define sys_NR_vmsplice 278
#define sys_NR_move_pages 279
#define sys_NR_utimensat 280
#define sys_NR_epoll_pwait 281
#define sys_NR_signalfd 282
#define sys_NR_timerfd_create 283
#define sys_NR_eventfd 284
#define sys_NR_fallocate 285
#define sys_NR_timerfd_settime 286
#define sys_NR_timerfd_gettime 287
#define sys_NR_accept4 288
#define sys_NR_signalfd4 289
#define sys_NR_eventfd2 290
#define sys_NR_epoll_create1 291
#define sys_NR_dup3 292
#define sys_NR_pipe2 293
#define sys_NR_inotify_init1 294
#define sys_NR_preadv 295
#define sys_NR_pwritev 296
#define sys_NR_rt_tgsigqueueinfo 297
#define sys_NR_perf_event_open 298
#define sys_NR_recvmmsg 299
#define sys_NR_fanotify_init 300
#define sys_NR_fanotify_mark 301
#define sys_NR_prlimit64 302
#define sys_NR_name_to_handle_at 303
#define sys_NR_open_by_handle_at 304
#define sys_NR_clock_adjtime 305
#define sys_NR_syncfs 306
#define sys_NR_sendmmsg 307
#define sys_NR_setns 308
#define sys_NR_getcpu 309
#define sys_NR_process_vm_readv 310
#define sys_NR_process_vm_writev 311
#define sys_NR_kcmp 312
#define sys_NR_finit_module 313
#define sys_NR_sched_setattr 314
#define sys_NR_sched_getattr 315
#define sys_NR_renameat2 316
#define sys_NR_seccomp 317
#define sys_NR_getrandom 318
#define sys_NR_memfd_create 319
#define sys_NR_kexec_file_load 320
#define sys_NR_bpf 321
#define sys_NR_execveat 322
#define sys_NR_userfaultfd 323
#define sys_NR_membarrier 324
#define sys_NR_mlock2 325
#define sys_NR_copy_file_range 326
#define sys_NR_preadv2 327
#define sys_NR_pwritev2 328
#define sys_NR_pkey_mprotect 329
#define sys_NR_pkey_alloc 330
#define sys_NR_pkey_free 331
#define sys_NR_statx 332
#define sys_NR_io_pgetevents 333
#define sys_NR_rseq 334
#define sys_NR_pidfd_send_signal 424
#define sys_NR_io_uring_setup 425
#define sys_NR_io_uring_enter 426
#define sys_NR_io_uring_register 427
#define sys_NR_open_tree 428
#define sys_NR_move_mount 429
#define sys_NR_fsopen 430
#define sys_NR_fsconfig 431
#define sys_NR_fsmount 432
#define sys_NR_fspick 433
#define sys_NR_pidfd_open 434
#define sys_NR_clone3 435
#define sys_NR_close_range 436
#define sys_NR_openat2 437
#define sys_NR_pidfd_getfd 438
#define sys_NR_faccessat2 439
#define sys_NR_process_madvise 440
#define sys_NR_epoll_pwait2 441
#define sys_NR_mount_setattr 442
#define sys_NR_quotactl_fd 443
#define sys_NR_landlock_create_ruleset 444
#define sys_NR_landlock_add_rule 445
#define sys_NR_landlock_restrict_self 446
#define sys_NR_memfd_secret 447
#define sys_NR_process_mrelease 448

#else

#define sys_NR_io_setup 0
#define sys_NR_io_destroy 1
#define sys_NR_io_submit 2
#define sys_NR_io_cancel 3
#define sys_NR_io_getevents 4 // Not on Risc-V.
#define sys_NR_setxattr 5
#define sys_NR_lsetxattr 6
#define sys_NR_fsetxattr 7
#define sys_NR_getxattr 8
#define sys_NR_lgetxattr 9
#define sys_NR_fgetxattr 10
#define sys_NR_listxattr 11
#define sys_NR_llistxattr 12
#define sys_NR_flistxattr 13
#define sys_NR_removexattr 14
#define sys_NR_lremovexattr 15
#define sys_NR_fremovexattr 16
#define sys_NR_getcwd 17
#define sys_NR_lookup_dcookie 18
#define sys_NR_eventfd2 19
#define sys_NR_epoll_create1 20
#define sys_NR_epoll_ctl 21
#define sys_NR_epoll_pwait 22
#define sys_NR_dup 23
#define sys_NR_dup3 24
#define sys_NR_fcntl 25
#define sys_NR_inotify_init1 26
#define sys_NR_inotify_add_watch 27
#define sys_NR_inotify_rm_watch 28
#define sys_NR_ioctl 29
#define sys_NR_ioprio_set 30
#define sys_NR_ioprio_get 31
#define sys_NR_flock 32
#define sys_NR_mknodat 33
#define sys_NR_mkdirat 34
#define sys_NR_unlinkat 35
#define sys_NR_symlinkat 36
#define sys_NR_linkat 37
#define sys_NR_renameat 38 // Not on Risc-V.
#define sys_NR_umount2 39
#define sys_NR_mount 40
#define sys_NR_pivot_root 41
#define sys_NR_nfsservctl 42
#define sys_NR_statfs 43
#define sys_NR_fstatfs 44
#define sys_NR_truncate 45
#define sys_NR_ftruncate 46
#define sys_NR_fallocate 47
#define sys_NR_faccessat 48
#define sys_NR_chdir 49
#define sys_NR_fchdir 50
#define sys_NR_chroot 51
#define sys_NR_fchmod 52
#define sys_NR_fchmodat 53
#define sys_NR_fchownat 54
#define sys_NR_fchown 55
#define sys_NR_openat 56
#define sys_NR_close 57
#define sys_NR_vhangup 58
#define sys_NR_pipe2 59
#define sys_NR_quotactl 60
#define sys_NR_getdents64 61
#define sys_NR_lseek 62
#define sys_NR_read 63
#define sys_NR_write 64
#define sys_NR_readv 65
#define sys_NR_writev 66
#define sys_NR_pread64 67
#define sys_NR_pwrite64 68
#define sys_NR_preadv 69
#define sys_NR_pwritev 70
#define sys_NR_sendfile 71
#define sys_NR_pselect6 72
#define sys_NR_ppoll 73
#define sys_NR_signalfd4 74
#define sys_NR_vmsplice 75
#define sys_NR_splice 76
#define sys_NR_tee 77
#define sys_NR_readlinkat 78
#define sys_NR_newfstatat 79
#define sys_NR_fstat 80
#define sys_NR_sync 81
#define sys_NR_fsync 82
#define sys_NR_fdatasync 83
#define sys_NR_sync_file_range 84
#define sys_NR_timerfd_create 85
#define sys_NR_timerfd_settime 86
#define sys_NR_timerfd_gettime 87
#define sys_NR_utimensat 88
#define sys_NR_acct 89
#define sys_NR_capget 90
#define sys_NR_capset 91
#define sys_NR_personality 92
#define sys_NR_exit 93
#define sys_NR_exit_group 94
#define sys_NR_waitid 95
#define sys_NR_set_tid_address 96
#define sys_NR_unshare 97
#define sys_NR_futex 98
#define sys_NR_set_robust_list 99
#define sys_NR_get_robust_list 100
#define sys_NR_nanosleep 101
#define sys_NR_getitimer 102
#define sys_NR_setitimer 103
#define sys_NR_kexec_load 104
#define sys_NR_init_module 105
#define sys_NR_delete_module 106
#define sys_NR_timer_create 107
#define sys_NR_timer_gettime 108
#define sys_NR_timer_getoverrun 109
#define sys_NR_timer_settime 110
#define sys_NR_timer_delete 111
#define sys_NR_clock_settime 112
#define sys_NR_clock_gettime 113
#define sys_NR_clock_getres 114
#define sys_NR_clock_nanosleep 115
#define sys_NR_syslog 116
#define sys_NR_ptrace 117
#define sys_NR_sched_setparam 118
#define sys_NR_sched_setscheduler 119
#define sys_NR_sched_getscheduler 120
#define sys_NR_sched_getparam 121
#define sys_NR_sched_setaffinity 122
#define sys_NR_sched_getaffinity 123
#define sys_NR_sched_yield 124
#define sys_NR_sched_get_priority_max 125
#define sys_NR_sched_get_priority_min 126
#define sys_NR_sched_rr_get_interval 127
#define sys_NR_restart_syscall 128
#define sys_NR_kill 129
#define sys_NR_tkill 130
#define sys_NR_tgkill 131
#define sys_NR_sigaltstack 132
#define sys_NR_rt_sigsuspend 133
#define sys_NR_rt_sigaction 134
#define sys_NR_rt_sigprocmask 135
#define sys_NR_rt_sigpending 136
#define sys_NR_rt_sigtimedwait 137
#define sys_NR_rt_sigqueueinfo 138
#define sys_NR_rt_sigreturn 139
#define sys_NR_setpriority 140
#define sys_NR_getpriority 141
#define sys_NR_reboot 142
#define sys_NR_setregid 143
#define sys_NR_setgid 144
#define sys_NR_setreuid 145
#define sys_NR_setuid 146
#define sys_NR_setresuid 147
#define sys_NR_getresuid 148
#define sys_NR_setresgid 149
#define sys_NR_getresgid 150
#define sys_NR_setfsuid 151
#define sys_NR_setfsgid 152
#define sys_NR_times 153
#define sys_NR_setpgid 154
#define sys_NR_getpgid 155
#define sys_NR_getsid 156
#define sys_NR_setsid 157
#define sys_NR_getgroups 158
#define sys_NR_setgroups 159
#define sys_NR_uname 160
#define sys_NR_sethostname 161
#define sys_NR_setdomainname 162
#define sys_NR_getrlimit 163
#define sys_NR_setrlimit 164
#define sys_NR_getrusage 165
#define sys_NR_umask 166
#define sys_NR_prctl 167
#define sys_NR_getcpu 168
#define sys_NR_gettimeofday 169
#define sys_NR_settimeofday 170
#define sys_NR_adjtimex 171
#define sys_NR_getpid 172
#define sys_NR_getppid 173
#define sys_NR_getuid 174
#define sys_NR_geteuid 175
#define sys_NR_getgid 176
#define sys_NR_getegid 177
#define sys_NR_gettid 178
#define sys_NR_sysinfo 179
#define sys_NR_mq_open 180
#define sys_NR_mq_unlink 181
#define sys_NR_mq_timedsend 182
#define sys_NR_mq_timedreceive 183
#define sys_NR_mq_notify 184
#define sys_NR_mq_getsetattr 185
#define sys_NR_msgget 186
#define sys_NR_msgctl 187
#define sys_NR_msgrcv 188
#define sys_NR_msgsnd 189
#define sys_NR_semget 190
#define sys_NR_semctl 191
#define sys_NR_semtimedop 192
#define sys_NR_semop 193
#define sys_NR_shmget 194
#define sys_NR_shmctl 195
#define sys_NR_shmat 196
#define sys_NR_shmdt 197
#define sys_NR_socket 198
#define sys_NR_socketpair 199
#define sys_NR_bind 200
#define sys_NR_listen 201
#define sys_NR_accept 202
#define sys_NR_connect 203
#define sys_NR_getsockname 204
#define sys_NR_getpeername 205
#define sys_NR_sendto 206
#define sys_NR_recvfrom 207
#define sys_NR_setsockopt 208
#define sys_NR_getsockopt 209
#define sys_NR_shutdown 210
#define sys_NR_sendmsg 211
#define sys_NR_recvmsg 212
#define sys_NR_readahead 213
#define sys_NR_brk 214
#define sys_NR_munmap 215
#define sys_NR_mremap 216
#define sys_NR_add_key 217
#define sys_NR_request_key 218
#define sys_NR_keyctl 219
#define sys_NR_clone 220
#define sys_NR_execve 221
#define sys_NR_mmap 222
#define sys_NR_fadvise64 223
#define sys_NR_swapon 224
#define sys_NR_swapoff 225
#define sys_NR_mprotect 226
#define sys_NR_msync 227
#define sys_NR_mlock 228
#define sys_NR_munlock 229
#define sys_NR_mlockall 230
#define sys_NR_munlockall 231
#define sys_NR_mincore 232
#define sys_NR_madvise 233
#define sys_NR_remap_file_pages 234
#define sys_NR_mbind 235
#define sys_NR_get_mempolicy 236
#define sys_NR_set_mempolicy 237
#define sys_NR_migrate_pages 238
#define sys_NR_move_pages 239
#define sys_NR_rt_tgsigqueueinfo 240
#define sys_NR_perf_event_open 241
#define sys_NR_accept4 242
#define sys_NR_recvmmsg 243
#define sys_NR_wait4 260
#define sys_NR_prlimit64 261
#define sys_NR_fanotify_init 262
#define sys_NR_fanotify_mark 263
#define sys_NR_name_to_handle_at 264
#define sys_NR_open_by_handle_at 265
#define sys_NR_clock_adjtime 266
#define sys_NR_syncfs 267
#define sys_NR_setns 268
#define sys_NR_sendmmsg 269
#define sys_NR_process_vm_readv 270
#define sys_NR_process_vm_writev 271
#define sys_NR_kcmp 272
#define sys_NR_finit_module 273
#define sys_NR_sched_setattr 274
#define sys_NR_sched_getattr 275
#define sys_NR_renameat2 276
#define sys_NR_seccomp 277
#define sys_NR_getrandom 278
#define sys_NR_memfd_create 279
#define sys_NR_bpf 280
#define sys_NR_execveat 281
#define sys_NR_userfaultfd 282
#define sys_NR_membarrier 283
#define sys_NR_mlock2 284
#define sys_NR_copy_file_range 285
#define sys_NR_preadv2 286
#define sys_NR_pwritev2 287
#define sys_NR_pkey_mprotect 288
#define sys_NR_pkey_alloc 289
#define sys_NR_pkey_free 290
#define sys_NR_statx 291
#define sys_NR_io_pgetevents 292
#define sys_NR_rseq 293
#define sys_NR_kexec_file_load 294
#define sys_NR_pidfd_send_signal 424
#define sys_NR_io_uring_setup 425
#define sys_NR_io_uring_enter 426
#define sys_NR_io_uring_register 427
#define sys_NR_open_tree 428
#define sys_NR_move_mount 429
#define sys_NR_fsopen 430
#define sys_NR_fsconfig 431
#define sys_NR_fsmount 432
#define sys_NR_fspick 433
#define sys_NR_pidfd_open 434
#define sys_NR_clone3 435
#define sys_NR_close_range 436
#define sys_NR_openat2 437
#define sys_NR_pidfd_getfd 438
#define sys_NR_faccessat2 439
#define sys_NR_process_madvise 440
#define sys_NR_epoll_pwait2 441
#define sys_NR_mount_setattr 442
#define sys_NR_quotactl_fd 443
#define sys_NR_landlock_create_ruleset 444
#define sys_NR_landlock_add_rule 445
#define sys_NR_landlock_restrict_self 446
#define sys_NR_memfd_secret 447 // Not on Risc-V.
#define sys_NR_process_mrelease 448

#if hc_AARCH64
#define sys_SYSCALL0(NUM) \
register int64_t num asm("x8") = (NUM); \
register int64_t ret asm("x0"); \
asm volatile( \
    "svc 0\n" \
    : "=r"(ret) \
    : "r"(num) \
    : "memory", "cc" \
);

#define sys_SYSCALL1(NUM, ARG1) \
register int64_t num asm("x8") = (NUM); \
register int64_t ret asm("x0") = (int64_t)(ARG1); \
asm volatile( \
    "svc 0\n" \
    : "+r"(ret) \
    : "r"(num) \
    : "memory", "cc" \
);

#define sys_SYSCALL2(NUM, ARG1, ARG2) \
register int64_t num asm("x8") = (NUM); \
register int64_t ret asm("x0") = (int64_t)(ARG1); \
register int64_t arg2 asm("x1") = (int64_t)(ARG2); \
asm volatile( \
    "svc 0\n" \
    : "+r"(ret) \
    : "r"(arg2), "r"(num) \
    : "memory", "cc" \
);

#define sys_SYSCALL3(NUM, ARG1, ARG2, ARG3) \
register int64_t num asm("x8") = (NUM); \
register int64_t ret asm("x0") = (int64_t)(ARG1); \
register int64_t arg2 asm("x1") = (int64_t)(ARG2); \
register int64_t arg3 asm("x2") = (int64_t)(ARG3); \
asm volatile( \
    "svc 0\n" \
    : "+r"(ret) \
    : "r"(arg2), "r"(arg3), "r"(num) \
    : "memory", "cc" \
);

#define sys_SYSCALL4(NUM, ARG1, ARG2, ARG3, ARG4) \
register int64_t num asm("x8") = (NUM); \
register int64_t ret asm("x0") = (int64_t)(ARG1); \
register int64_t arg2 asm("x1") = (int64_t)(ARG2); \
register int64_t arg3 asm("x2") = (int64_t)(ARG3); \
register int64_t arg4 asm("x3") = (int64_t)(ARG4); \
asm volatile( \
    "svc 0\n" \
    : "+r"(ret) \
    : "r"(arg2), "r"(arg3), "r"(arg4), "r"(num) \
    : "memory", "cc" \
);

#define sys_SYSCALL5(NUM, ARG1, ARG2, ARG3, ARG4, ARG5) \
register int64_t num asm("x8") = (NUM); \
register int64_t ret asm("x0") = (int64_t)(ARG1); \
register int64_t arg2 asm("x1") = (int64_t)(ARG2); \
register int64_t arg3 asm("x2") = (int64_t)(ARG3); \
register int64_t arg4 asm("x3") = (int64_t)(ARG4); \
register int64_t arg5 asm("x4") = (int64_t)(ARG5); \
asm volatile( \
    "svc 0\n" \
    : "+r"(ret) \
    : "r"(arg2), "r"(arg3), "r"(arg4), "r"(arg5), "r"(num) \
    : "memory", "cc" \
);

#define sys_SYSCALL6(NUM, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
register int64_t num asm("x8") = (NUM); \
register int64_t ret asm("x0") = (int64_t)(ARG1); \
register int64_t arg2 asm("x1") = (int64_t)(ARG2); \
register int64_t arg3 asm("x2") = (int64_t)(ARG3); \
register int64_t arg4 asm("x3") = (int64_t)(ARG4); \
register int64_t arg5 asm("x4") = (int64_t)(ARG5); \
register int64_t arg6 asm("x5") = (int64_t)(ARG6); \
asm volatile( \
    "svc 0\n" \
    : "+r"(ret) \
    : "r"(arg2), "r"(arg3), "r"(arg4), "r"(arg5), "r"(arg6), "r"(num) \
    : "memory", "cc" \
);

#elif hc_RISCV64
#define sys_SYSCALL0(NUM) \
register int64_t num asm("a7") = (NUM); \
register int64_t ret asm("a0"); \
asm volatile( \
    "ecall\n" \
    : "=r"(ret) \
    : "r"(num) \
    : "memory", "cc" \
);

#define sys_SYSCALL1(NUM, ARG1) \
register int64_t num asm("a7") = (NUM); \
register int64_t ret asm("a0") = (int64_t)(ARG1); \
asm volatile( \
    "ecall\n" \
    : "+r"(ret) \
    : "r"(num) \
    : "memory", "cc" \
);

#define sys_SYSCALL2(NUM, ARG1, ARG2) \
register int64_t num asm("a7") = (NUM); \
register int64_t ret asm("a0") = (int64_t)(ARG1); \
register int64_t arg2 asm("a1") = (int64_t)(ARG2); \
asm volatile( \
    "ecall\n" \
    : "+r"(ret) \
    : "r"(arg2), "r"(num) \
    : "memory", "cc" \
);

#define sys_SYSCALL3(NUM, ARG1, ARG2, ARG3) \
register int64_t num asm("a7") = (NUM); \
register int64_t ret asm("a0") = (int64_t)(ARG1); \
register int64_t arg2 asm("a1") = (int64_t)(ARG2); \
register int64_t arg3 asm("a2") = (int64_t)(ARG3); \
asm volatile( \
    "ecall\n" \
    : "+r"(ret) \
    : "r"(arg2), "r"(arg3), "r"(num) \
    : "memory", "cc" \
);

#define sys_SYSCALL4(NUM, ARG1, ARG2, ARG3, ARG4) \
register int64_t num asm("a7") = (NUM); \
register int64_t ret asm("a0") = (int64_t)(ARG1); \
register int64_t arg2 asm("a1") = (int64_t)(ARG2); \
register int64_t arg3 asm("a2") = (int64_t)(ARG3); \
register int64_t arg4 asm("a3") = (int64_t)(ARG4); \
asm volatile( \
    "ecall\n" \
    : "+r"(ret) \
    : "r"(arg2), "r"(arg3), "r"(arg4), "r"(num) \
    : "memory", "cc" \
);

#define sys_SYSCALL5(NUM, ARG1, ARG2, ARG3, ARG4, ARG5) \
register int64_t num asm("a7") = (NUM); \
register int64_t ret asm("a0") = (int64_t)(ARG1); \
register int64_t arg2 asm("a1") = (int64_t)(ARG2); \
register int64_t arg3 asm("a2") = (int64_t)(ARG3); \
register int64_t arg4 asm("a3") = (int64_t)(ARG4); \
register int64_t arg5 asm("a4") = (int64_t)(ARG5); \
asm volatile( \
    "ecall\n" \
    : "+r"(ret) \
    : "r"(arg2), "r"(arg3), "r"(arg4), "r"(arg5), "r"(num) \
    : "memory", "cc" \
);

#define sys_SYSCALL6(NUM, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) \
register int64_t num asm("a7") = (NUM); \
register int64_t ret asm("a0") = (int64_t)(ARG1); \
register int64_t arg2 asm("a1") = (int64_t)(ARG2); \
register int64_t arg3 asm("a2") = (int64_t)(ARG3); \
register int64_t arg4 asm("a3") = (int64_t)(ARG4); \
register int64_t arg5 asm("a4") = (int64_t)(ARG5); \
register int64_t arg6 asm("a5") = (int64_t)(ARG6); \
asm volatile( \
    "ecall\n" \
    : "+r"(ret) \
    : "r"(arg2), "r"(arg3), "r"(arg4), "r"(arg5), "r"(arg6), "r"(num) \
    : "memory", "cc" \
);
#endif
#endif

hc_UNUSED
static hc_ALWAYS_INLINE int64_t sys_write(int32_t fd, const void *buf, int64_t count) {
    sys_SYSCALL3(sys_NR_write, fd, buf, count);
    return ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int64_t sys_writev(int32_t fd, const struct iovec *iov, int32_t iovlen) {
    sys_SYSCALL3(sys_NR_writev, fd, iov, iovlen);
    return ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_close(int32_t fd) {
    sys_SYSCALL1(sys_NR_close, fd);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_openat(int32_t dirfd, const void *pathname, uint32_t flags, uint16_t mode) {
    sys_SYSCALL4(sys_NR_openat, dirfd, pathname, flags, mode);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int64_t sys_read(int32_t fd, void *buf, int64_t count) {
    sys_SYSCALL3(sys_NR_read, fd, buf, count);
    return ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE _Noreturn void sys_exit(int32_t exitcode) {
    sys_SYSCALL1(sys_NR_exit, exitcode);
    hc_UNREACHABLE;
}

hc_UNUSED
static hc_ALWAYS_INLINE _Noreturn void sys_exit_group(int32_t exitcode) {
    sys_SYSCALL1(sys_NR_exit_group, exitcode);
    hc_UNREACHABLE;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_getpid(void) {
    sys_SYSCALL0(sys_NR_getpid);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_kill(int32_t pid, int32_t signal) {
    sys_SYSCALL2(sys_NR_kill, pid, signal);
    return (int32_t)ret;
}

#define sys_SIGMASK(SIGNAL) (1UL << ((SIGNAL) - 1))
hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_rt_sigprocmask(uint32_t how, uint64_t *set, uint64_t *oldset) {
    sys_SYSCALL4(sys_NR_rt_sigprocmask, how, set, oldset, 8);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_signalfd4(int32_t fd, const uint64_t *mask, uint32_t flags) {
    sys_SYSCALL4(sys_NR_signalfd4, fd, mask, 8, flags);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_rt_sigaction(int32_t sig, const struct sigaction *act, struct sigaction *oldact) {
    sys_SYSCALL4(sys_NR_rt_sigaction, sig, act, oldact, 8);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_socket(int32_t family, int32_t type, int32_t protocol) {
    sys_SYSCALL3(sys_NR_socket, family, type, protocol);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_setsockopt(int32_t fd, int32_t level, int32_t optname, const void *optval, int32_t optlen) {
    sys_SYSCALL5(sys_NR_setsockopt, fd, level, optname, optval, optlen);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_connect(int32_t fd, const void *addr, int32_t addrlen) {
    sys_SYSCALL3(sys_NR_connect, fd, addr, addrlen);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_bind(int32_t fd, const void *addr, int32_t addrlen) {
    sys_SYSCALL3(sys_NR_bind, fd, addr, addrlen);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_listen(int32_t fd, int32_t backlog) {
    sys_SYSCALL2(sys_NR_listen, fd, backlog);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_accept4(int32_t fd, void *addr, int32_t *restrict addrlen, uint32_t flags) {
    sys_SYSCALL4(sys_NR_accept4, fd, addr, addrlen, flags);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int64_t sys_sendto(int32_t fd, const void *buf, int64_t len, uint32_t flags, const void *addr, int32_t addrlen) {
    sys_SYSCALL6(sys_NR_sendto, fd, buf, len, flags, addr, addrlen);
    return ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int64_t sys_sendmsg(int32_t fd, const struct msghdr *msg, uint32_t flags) {
    sys_SYSCALL3(sys_NR_sendmsg, fd, msg, flags);
    return ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int64_t sys_recvfrom(int32_t fd, const void *restrict buf, int64_t len, uint32_t flags, const void *restrict addr, int32_t *restrict addrlen) {
    sys_SYSCALL6(sys_NR_recvfrom, fd, buf, len, flags, addr, addrlen);
    return ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_epoll_create1(uint32_t flags) {
    sys_SYSCALL1(sys_NR_epoll_create1, flags);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_epoll_ctl(int32_t epfd, int32_t op, int32_t fd, struct epoll_event *event) {
    sys_SYSCALL4(sys_NR_epoll_ctl, epfd, op, fd, event);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_epoll_pwait(int32_t epfd, struct epoll_event *events, int32_t maxevents, int32_t timeout, const int64_t *sigmask) {
    sys_SYSCALL6(sys_NR_epoll_pwait, epfd, events, maxevents, timeout, sigmask, sizeof(*sigmask));
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_timerfd_create(int32_t clockid, uint32_t flags) {
    sys_SYSCALL2(sys_NR_timerfd_create, clockid, flags);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_timerfd_settime(int32_t fd, uint32_t flags, const struct itimerspec *new, struct itimerspec *old) {
    sys_SYSCALL4(sys_NR_timerfd_settime, fd, flags, new, old);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE void *sys_mmap(void *addr, int64_t length, int32_t prot, uint32_t flags, int32_t fd, int64_t offset) {
    sys_SYSCALL6(sys_NR_mmap, addr, length, prot, flags, fd, offset);
    return (void *)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_munmap(void *addr, int64_t length) {
    sys_SYSCALL2(sys_NR_munmap, addr, length);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE void *sys_mremap(void *oldaddr, int64_t oldsize, int64_t newsize, uint32_t flags) {
    sys_SYSCALL5(sys_NR_mremap, oldaddr, oldsize, newsize, flags, NULL);
    return (void *)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_madvise(void *addr, int64_t size, uint32_t advice) {
    sys_SYSCALL3(sys_NR_madvise, addr, size, advice);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_clock_gettime(int32_t clock, struct timespec *time) {
    sys_SYSCALL2(sys_NR_clock_gettime, clock, time);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_clock_nanosleep(int32_t clock, uint32_t flags, const struct timespec *request, struct timespec *remain) {
    sys_SYSCALL4(sys_NR_clock_nanosleep, clock, flags, request, remain);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int64_t sys_getrandom(void *buf, int64_t buflen, uint32_t flags) {
    sys_SYSCALL3(sys_NR_getrandom, buf, buflen, flags);
    return ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_clone3(struct clone_args *args, uint64_t size) {
    sys_SYSCALL2(sys_NR_clone3, args, size);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_wait4(int32_t pid, int32_t *status, int32_t options) {
    sys_SYSCALL4(sys_NR_wait4, pid, status, options, NULL);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_futex(int32_t *addr, int32_t op, int32_t val, struct timespec *timeout, int32_t *addr2, int32_t val3) {
    sys_SYSCALL6(sys_NR_futex, addr, op, val, timeout, addr2, val3);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_setsid(void) {
    sys_SYSCALL0(sys_NR_setsid);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_setpgid(int32_t pid, int32_t pgid) {
    sys_SYSCALL2(sys_NR_setpgid, pid, pgid);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_getpgid(int32_t pid) {
    sys_SYSCALL1(sys_NR_getpgid, pid);
    return (int32_t)ret;
}

hc_UNUSED
static hc_ALWAYS_INLINE int32_t sys_ioctl(int32_t fd, uint32_t cmd, void *arg) {
    sys_SYSCALL3(sys_NR_ioctl, fd, cmd, arg);
    return (int32_t)ret;
}