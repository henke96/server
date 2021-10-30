static int64_t debug_strlen(const char *str) {
    const char *c = str;
    while (*c != '\0') ++c;
    return c - str;
}

static void debug_printNum(const char *pre, int64_t num, const char *post) {
    char buffer[20];

    char *pos = &buffer[sizeof(buffer) - 1];
    uint64_t n = num < 0 ? (uint64_t)(-num) : (uint64_t)(num);
    int64_t numLen = 0;
    do {
        *pos-- = (char)('0' + n % 10);
        n /= 10;
        ++numLen;
    } while (n != 0);

    if (num < 0) *pos-- = '-';
    nolibc_write(STDOUT_FILENO, pre, debug_strlen(pre));
    nolibc_write(STDOUT_FILENO, pos + 1, numLen);
    nolibc_write(STDOUT_FILENO, post, debug_strlen(post));
}

#ifdef debug_NDEBUG
#define debug_assert(x) ((void)0)
#else
static noreturn void debug_failAssert(const char *expression, const char *file, const char *function, int32_t line) {
    nolibc_write(STDOUT_FILENO, "Assertion failed: ", 18);
    nolibc_write(STDOUT_FILENO, expression, debug_strlen(expression));
    nolibc_write(STDOUT_FILENO, " (", 2);
    nolibc_write(STDOUT_FILENO, file, debug_strlen(file));
    nolibc_write(STDOUT_FILENO, ": ", 2);
    nolibc_write(STDOUT_FILENO, function, debug_strlen(function));
    debug_printNum(": ", line, ")\n");
    nolibc_kill(nolibc_getpid(), SIGABRT);
    nolibc_exit_group(137);
}
#define debug_assert(x) ((void)((x) || (debug_failAssert(#x, __FILE__, __func__, __LINE__), 0)))
#endif
