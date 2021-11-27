hc_UNUSED
static int64_t debug_strlen(const char *str) {
    const char *c = str;
    while (*c != '\0') ++c;
    return c - str;
}

hc_UNUSED
static void debug_printNum(const char *pre, int64_t num, const char *post) {
    char buffer[20];

    char *pos = &buffer[sizeof(buffer) - 1];
    uint64_t n = num < 0 ? -((uint64_t)num) : (uint64_t)num;
    int64_t numLen = 0;
    do {
        *pos-- = (char)('0' + n % 10);
        n /= 10;
        ++numLen;
    } while (n != 0);

    if (num < 0) {
        *pos-- = '-';
        ++numLen;
    }
    hc_write(STDOUT_FILENO, pre, debug_strlen(pre));
    hc_write(STDOUT_FILENO, pos + 1, numLen);
    hc_write(STDOUT_FILENO, post, debug_strlen(post));
}

hc_UNUSED
static void debug_printStr(const char *pre, const char *str, const char *post, int64_t strlen) {
    hc_write(STDOUT_FILENO, pre, debug_strlen(pre));
    if (strlen < 0) strlen = debug_strlen(str);
    hc_write(STDOUT_FILENO, str, strlen);
    hc_write(STDOUT_FILENO, post, debug_strlen(post));
}

#ifdef debug_NDEBUG
#define debug_ASSERT(X) ((void)0)
#else
hc_UNUSED
static noreturn void debug_failAssert(const char *expression, const char *file, const char *function, int32_t line) {
    hc_write(STDOUT_FILENO, "Assertion failed: ", 18);
    hc_write(STDOUT_FILENO, expression, debug_strlen(expression));
    hc_write(STDOUT_FILENO, " (", 2);
    hc_write(STDOUT_FILENO, file, debug_strlen(file));
    hc_write(STDOUT_FILENO, ": ", 2);
    hc_write(STDOUT_FILENO, function, debug_strlen(function));
    debug_printNum(": ", line, ")\n");
    hc_kill(hc_getpid(), SIGABRT);
    hc_exit_group(137);
}
#define debug_ASSERT(X) ((void)((X) || (debug_failAssert(#X, __FILE__, __func__, __LINE__), 0)))
#endif
