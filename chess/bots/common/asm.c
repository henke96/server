static hc_ALWAYS_INLINE uint64_t asm_tzcnt(uint64_t x) {
    uint64_t result;
    asm("tzcnt %1, %0\n"
        : "=r"(result)
        : "r"(x)
    );
    return result;
}

static hc_ALWAYS_INLINE uint64_t asm_lzcnt(uint64_t x) {
    uint64_t result;
    asm("lzcnt %1, %0\n"
        : "=r"(result)
        : "r"(x)
    );
    return result;
}

static hc_ALWAYS_INLINE uint64_t asm_popcnt(uint64_t x) {
    uint64_t result;
    asm("popcnt %1, %0\n"
        : "=r"(result)
        : "r"(x)
    );
    return result;
}

static hc_ALWAYS_INLINE uint64_t asm_blsr(uint64_t x) {
    uint64_t result;
    asm("blsr %1, %0\n"
        : "=r"(result)
        : "r"(x)
    );
    return result;
}

static hc_ALWAYS_INLINE uint64_t asm_blsi(uint64_t x) {
    uint64_t result;
    asm("blsi %1, %0\n"
        : "=r"(result)
        : "r"(x)
    );
    return result;
}

static hc_ALWAYS_INLINE uint64_t asm_pext(uint64_t x, uint64_t mask) {
    uint64_t result;
    asm("pext %2, %1, %0\n"
        : "=r"(result)
        : "r"(x), "r"(mask)
    );
    return result;
}