// Returns pid, or negative error code.
int32_t sys_clone(struct clone_args *args, uint64_t size, void (*childfunc)(void *), void *childarg);
#if hc_X86_64
asm(
    ".section .text\n"
    ".local sys_clone\n"
    "sys_clone:\n"
    "mov %rcx, %r10\n"
    "mov $435, %eax\n"
    "syscall\n"
    "test %eax, %eax\n"
    "jz 1f\n"
    "ret\n"
    "1: mov %r10, %rdi\n"
    "call *%rdx\n"
);
#elif hc_AARCH64
asm(
    ".section .text\n"
    ".local sys_clone\n"
    "sys_clone:\n"
    "mov x8, 435\n"
    "svc 0\n"
    "cbz x0, 1f\n"
    "ret\n"
    "1: mov x0, x3\n"
    "blr x2\n"
);
#elif hc_RISCV64
asm(
    ".section .text\n"
    ".local sys_clone\n"
    "sys_clone:\n"
    "li a7, 435\n"
    "ecall\n"
    "beq a0, x0, 1f\n"
    "ret\n"
    "1: mv a0, a3\n"
    "jalr a2\n"
);
#endif
