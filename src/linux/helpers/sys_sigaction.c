#if hc_X86_64
void sys_sigaction_restore(void);
asm(
    ".section .text\n"
    ".local sys_sigaction_restore\n"
    "sys_sigaction_restore:\n"
    "mov $15, %rax\n"
    "syscall\n"
);
#endif

// Architecture independent version of sys_rt_sigaction().
// Just pretend the `sa_restorer` field of `struct sigaction` doesn't exist when using this.
static hc_ALWAYS_INLINE int32_t sys_sigaction(int32_t sig, struct sigaction *act, struct sigaction *oldact) {
#if hc_X86_64
    act->sa_flags |= SA_RESTORER;
    act->sa_restorer = sys_sigaction_restore;
#endif
    return sys_rt_sigaction(sig, act, oldact);
}