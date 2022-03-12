
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

// Expects `buffer` to have 1 or more digits followed by `terminator`.
// Returns the number of characters in the parsed number (0 on errors).
// Sets `*number` to the parsed value if successful.
hc_UNUSED static int32_t util_strToUint(char *buffer, char terminator, uint64_t *number) {
    uint64_t result = 0;
    int32_t i = 0;
    do {
        int64_t digitValue = (buffer[i] - '0');
        if (
            (digitValue < 0 || digitValue > 9) ||
            (result > (UINT64_MAX - (uint64_t)digitValue) / 10)
        ) return 0; // Not a digit or overflow.

        result = result * 10 + (uint64_t)digitValue;
        ++i;
    } while (buffer[i] != terminator);

    *number = result;
    return i;
}

// Expects `buffer` to have 1 or more digits followed by `terminator`, optionally starting with a minus sign.
// Returns the number of characters in the parsed number (0 on errors).
// Sets `*number` to the parsed value if successful.
hc_UNUSED static int32_t util_strToInt(char *buffer, char terminator, int64_t *number) {
    if (buffer[0] == '-') {
        uint64_t value;
        int32_t parsed = util_strToUint(&buffer[1], terminator, &value);
        if (parsed == 0 || value > (uint64_t)INT64_MAX + 1) return 0;
        *number = (int64_t)-value;
        return 1 + parsed;
    } else {
        uint64_t value;
        int32_t parsed = util_strToUint(buffer, terminator, &value);
        if (parsed == 0 || value > (uint64_t)INT64_MAX) return 0;
        *number = (int64_t)value;
        return parsed;
    }
}