hc_UNUSED
static void debug_printNum(const char *pre, int64_t num, const char *post) {
    char buffer[util_INT64_MAX_CHARS];
    char *numStr = util_intToStr(&buffer[util_INT64_MAX_CHARS], num);

    struct iovec iov[3] = {
        {
            .iov_base = (char *)pre,
            .iov_len = util_cstrLen(pre)
        }, {
            .iov_base = numStr,
            .iov_len = (int64_t)(&buffer[util_INT64_MAX_CHARS] - numStr)
        }, {
            .iov_base = (char *)post,
            .iov_len = util_cstrLen(post)
        }
    };
    hc_writev(STDOUT_FILENO, &iov[0], 3);
}

hc_UNUSED
static void debug_printStr(const char *pre, const char *str, const char *post, int64_t strlen) {
    struct iovec iov[3] = {
        {
            .iov_base = (char *)pre,
            .iov_len = util_cstrLen(pre)
        }, {
            .iov_base = (char *)str,
            .iov_len = strlen
        }, {
            .iov_base = (char *)post,
            .iov_len = util_cstrLen(post)
        }
    };
    hc_writev(STDOUT_FILENO, &iov[0], 3);
}

#ifdef debug_NDEBUG
#define debug_ASSERT(X) ((void)0)
#else
hc_UNUSED
static noreturn void debug_failAssert(const char *expression, const char *file, const char *function, int32_t line) {
    char buffer[util_INT32_MAX_CHARS];
    char *lineStr = util_intToStr(&buffer[util_INT32_MAX_CHARS], line);

    static const char start[18] = "Assertion failed: ";
    static const char parenStart[2] = " (";
    static const char colon[2] = ": ";
    static const char end[2] = ")\n";

    struct iovec iov[9] = {
        {
            .iov_base = (char *)&start[0],
            .iov_len = sizeof(start)
        }, {
            .iov_base = (char *)expression,
            .iov_len = util_cstrLen(expression)
        }, {
            .iov_base = (char *)&parenStart[0],
            .iov_len = sizeof(parenStart)
        }, {
            .iov_base = (char *)file,
            .iov_len = util_cstrLen(file)
        }, {
            .iov_base = (char *)&colon[0],
            .iov_len = 1
        }, {
            .iov_base = lineStr,
            .iov_len = (int64_t)(&buffer[util_INT32_MAX_CHARS] - lineStr)
        }, {
            .iov_base = (char *)&colon[0],
            .iov_len = sizeof(colon)
        }, {
            .iov_base = (char *)function,
            .iov_len = util_cstrLen(function)
        }, {
            .iov_base = (char *)&end[0],
            .iov_len = sizeof(end)
        }
    };

    hc_writev(STDOUT_FILENO, &iov[0], 9);
    hc_kill(hc_getpid(), SIGABRT);
    hc_exit_group(137);
}
#define debug_ASSERT(X) ((void)((X) || (debug_failAssert(#X, __FILE__, __func__, __LINE__), 0)))
#endif
