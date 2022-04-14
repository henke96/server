// Thread local storage helpers.

hc_UNUSED
static struct elf_programHeader *tls_findProgramHeader(const uint64_t *auxv) {
    struct elf_programHeader *programHeaders = NULL;
    uint64_t programHeadersLength = 0;
    for (int32_t i = 0; auxv[i] != AT_NULL; i += 2) {
        switch (auxv[i]) {
            case AT_PHDR: {
                programHeaders = (struct elf_programHeader *)auxv[i + 1];
                if (programHeadersLength > 0) goto foundProgramHeaders;
                break;
            }
            case AT_PHNUM: {
                programHeadersLength = auxv[i + 1];
                if (programHeaders != NULL) goto foundProgramHeaders;
                break;
            }
        }
    }
    return NULL;

    foundProgramHeaders:
    for (uint64_t i = 0; i < programHeadersLength; ++i) {
        if (programHeaders[i].type == elf_PT_TLS) return &programHeaders[i];
    }
    return NULL;
}

#if hc_X86_64
    // X86_64 ABI has the thread pointer point to its own value, so we need 8 more bytes.
    #define tls_AREA_SIZE(SEGMENT_MEMORY_SIZE) (SEGMENT_MEMORY_SIZE + 8)
#else
    #define tls_AREA_SIZE(SEGMENT_MEMORY_SIZE) (SEGMENT_MEMORY_SIZE)
#endif

// Initialise a tls area.
// `tlsArea` must have an alignment of at least `tlsProgramHeader->segmentAlignment` and must be
// at least `tls_AREA_SIZE(tlsProgramHeader->segmentMemorySize)` bytes long.
// Bytes at `tlsProgramHeader->segmentFileSize` and after must be zero.
// Returns the thread pointer for the area.
hc_UNUSED static uint64_t tls_initArea(struct elf_programHeader *tlsProgramHeader, void *tlsArea) {
    hc_MEMCPY(tlsArea, (void *)tlsProgramHeader->virtualAddress, tlsProgramHeader->segmentFileSize);
#if hc_X86_64
    uint64_t threadPointer = (uint64_t)tlsArea + hc_ALIGN_FORWARD(tlsProgramHeader->segmentMemorySize, tlsProgramHeader->segmentAlignment);
    *(uint64_t *)threadPointer = threadPointer;
    return threadPointer;
#elif hc_AARCH64
    return (uint64_t)tlsArea - 16;
#elif hc_RISCV64
    return (uint64_t)tlsArea;
#endif
}

// Set thread pointer for current thread.
hc_UNUSED static hc_ALWAYS_INLINE void tls_setThreadPointer(uint64_t threadPointer) {
#if hc_X86_64
    sys_SYSCALL2(sys_NR_arch_prctl, ARCH_SET_FS, threadPointer)
#elif hc_AARCH64
    asm volatile(
        "msr tpidr_el0, %0\n"
        :
        : "r"(threadPointer)
    );
#elif hc_RISCV64
    asm volatile(
        "mv tp, %0\n"
        :
        : "r"(threadPointer)
    );
#endif
}