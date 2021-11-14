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
#if hc_X86_64
asm(
    ".section .text\n"
    ".global _start\n"
    "_start:\n"
    "pop %rdi\n"                // argc   (first arg, %rdi)
    "mov %rsp, %rsi\n"          // argv[] (second arg, %rsi)
    "lea 8(%rsi,%rdi,8),%rdx\n" // then a NULL then envp (third arg, %rdx)
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
    "lsl x2, x0, 3\n"             // envp (x2) = 8*argc ...
    "add x2, x2, 8\n"             //           + 8 (skip null)
    "add x2, x2, x1\n"            //           + argv
    "and sp, x1, -16\n"           // sp must be 16-byte aligned in the callee
    "bl main\n"                   // main() returns the status code, we'll exit with it
    "mov x8, 94\n"                // NR_exit_group == 94
    "svc #0\n"
);
#endif