// Entry point
#if hc_X86_64
asm(
    ".section .text\n"
    ".global _start\n"
    "_start:\n"
    "pop %rdi\n"                // argc   (first arg, %rdi)
    "mov %rsp, %rsi\n"          // argv[] (second arg, %rsi)
    "xor %ebp, %ebp\n"          // the deepest stack frame should be zero
    "and $-16, %rsp\n"          // x86 ABI: esp must be 16-byte aligned
    "call main\n"               // main() returns the status code, we'll exit with it
    "mov %eax, %edi\n"          // retrieve exit code
    "mov $231, %eax\n"          // NR_exit_group == 231
    "syscall\n"                 // really exit
);
#elif hc_AARCH64
asm(
    ".section .text\n"
    ".global _start\n"
    "_start:\n"
    "ldr x0, [sp]\n"              // argc (x0) was in the stack
    "add x1, sp, 8\n"             // argv (x1) = sp
    "and sp, x1, -16\n"           // sp must be 16-byte aligned in the callee
    "bl main\n"                   // main() returns the status code, we'll exit with it
    "mov x8, 94\n"                // NR_exit_group == 94
    "svc #0\n"
);
#endif