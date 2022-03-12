
hc_UNUSED
static int64_t util_cstrLen(const char *cstring) {
    const char *c = cstring;
    for (; *c != '\0'; ++c);
    return c - cstring;
}

#define util_INT32_MAX_CHARS 11
#define util_UINT32_MAX_CHARS 10
#define util_INT64_MAX_CHARS 20
#define util_UINT64_MAX_CHARS 20
// Writes max 20 characters.
// `bufferEnd` points 1 past where last digit is written.
// Returns pointer to first character of result.
hc_UNUSED static char *util_intToStr(char *bufferEnd, int64_t number) {
    uint64_t n = number < 0 ? -((uint64_t)number) : (uint64_t)number;
    do {
        *--bufferEnd = (char)('0' + n % 10);
        n /= 10;
    } while (n != 0);

    if (number < 0) *--bufferEnd = '-';
    return bufferEnd;
}

// Writes max 20 characters.
// `bufferEnd` points 1 past where last digit is written.
// Returns pointer to first character of result.
hc_UNUSED static char *util_uintToStr(char *bufferEnd, uint64_t number) {
    do {
        *--bufferEnd = (char)('0' + number % 10);
        number /= 10;
    } while (number != 0);
    return bufferEnd;
}