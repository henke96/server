#if hc_X86_64
#define vdso_CLOCK_GETTIME "__vdso_clock_gettime"
#define vdso_GETTIMEOFDAY "__vdso_gettimeofday"
#elif hc_AARCH64
#define vdso_CLOCK_GETTIME "__kernel_clock_gettime"
#define vdso_GETTIMEOFDAY "__kernel_gettimeofday"
#elif hc_RISCV64
#define vdso_CLOCK_GETTIME "__vdso_clock_gettime"
#define vdso_GETTIMEOFDAY "__vdso_gettimeofday"
#endif

static void *vdso_lookup(const uint64_t *auxv, const char *symbolName) {
    // Find vDSO elf header.
    struct elf_header *elfHeader;
    for (int32_t i = 0; auxv[i] != AT_NULL; i += 2) {
        if (auxv[i] == AT_SYSINFO_EHDR) {
            elfHeader = (struct elf_header *)auxv[i + 1];
            goto foundVDSO;
        }
    }
    return NULL;
    foundVDSO:;

    // Find dynamic section.
    struct elf_dynamicInfo *dynamicSection;
    struct elf_programHeader *programHeaders = (void *)((char *)elfHeader + elfHeader->programHeadersOffset);
    for (int32_t i = 0; i < elfHeader->programHeadersLength; ++i) {
        if (programHeaders[i].type == elf_PT_DYNAMIC) {
            dynamicSection = (void *)((char *)elfHeader + programHeaders[i].fileOffset);
            goto foundDynamicSection;
        }
    }
    return NULL;
    foundDynamicSection:;

    // Find dynamic string table, symbol table and hash table.
    char *strings = NULL;
    struct elf_symbol *symbols = NULL;
    struct elf_hashTable *hashTable = NULL;
    for (int32_t i = 0; dynamicSection[i].tag != elf_DT_NULL; ++i) {
        void *pointer = (char *)elfHeader + dynamicSection[i].value;
        switch (dynamicSection[i].tag) {
            case elf_DT_STRTAB: strings = pointer; break;
            case elf_DT_SYMTAB: symbols = pointer; break;
            case elf_DT_HASH: hashTable = pointer; break;
        }
    }
    if (strings == NULL || symbols == NULL || hashTable == NULL) return NULL;

    // Find the requested symbol.
    for (uint32_t i = 0; i < hashTable->numChains; ++i) {
        if (util_cstrCmp(symbolName, &strings[symbols[i].nameIndex]) == 0) {
            return (char *)elfHeader + symbols[i].value;
        }
    }
    return NULL;
}