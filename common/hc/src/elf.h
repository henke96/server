// Values for `elf_programHeader.type`.
#define elf_PT_NULL 0
#define elf_PT_LOAD 1
#define elf_PT_DYNAMIC 2
#define elf_PT_INTERP 3
#define elf_PT_NOTE 4
#define elf_PT_SHLIB 5
#define elf_PT_PHDR 6
#define elf_PT_TLS 7
#define elf_PT_LOOS 0x60000000
#define elf_PT_HIOS 0x6fffffff
#define elf_PT_LOPROC 0x70000000
#define elf_PT_HIPROC 0x7fffffff
#define elf_PT_GNU_EH_FRAME 0x6474e550
#define elf_PT_GNU_PROPERTY 0x6474e553
#define elf_PT_GNU_STACK (elf_PT_LOOS + 0x474e551)

// Values for `elf_dynamicInfo.tag`.
#define elf_DT_NULL 0
#define elf_DT_NEEDED 1
#define elf_DT_PLTRELSZ 2
#define elf_DT_PLTGOT 3
#define elf_DT_HASH 4
#define elf_DT_STRTAB 5
#define elf_DT_SYMTAB 6
#define elf_DT_RELA 7
#define elf_DT_RELASZ 8
#define elf_DT_RELAENT 9
#define elf_DT_STRSZ 10
#define elf_DT_SYMENT 11
#define elf_DT_INIT 12
#define elf_DT_FINI 13
#define elf_DT_SONAME 14
#define elf_DT_RPATH  15
#define elf_DT_SYMBOLIC 16
#define elf_DT_REL 17
#define elf_DT_RELSZ 18
#define elf_DT_RELENT 19
#define elf_DT_PLTREL 20
#define elf_DT_DEBUG 21
#define elf_DT_TEXTREL 22
#define elf_DT_JMPREL 23
#define elf_DT_ENCODING 32
#define elf_OLD_DT_LOOS 0x60000000
#define elf_DT_LOOS 0x6000000d
#define elf_DT_HIOS 0x6ffff000
#define elf_DT_VALRNGLO 0x6ffffd00
#define elf_DT_VALRNGHI 0x6ffffdff
#define elf_DT_ADDRRNGLO 0x6ffffe00
#define elf_DT_ADDRRNGHI 0x6ffffeff
#define elf_DT_VERSYM 0x6ffffff0
#define elf_DT_RELACOUNT 0x6ffffff9
#define elf_DT_RELCOUNT 0x6ffffffa
#define elf_DT_FLAGS_1 0x6ffffffb
#define elf_DT_VERDEF 0x6ffffffc
#define elf_DT_VERDEFNUM 0x6ffffffd
#define elf_DT_VERNEED 0x6ffffffe
#define elf_DT_VERNEEDNUM 0x6fffffff
#define elf_OLD_DT_HIOS 0x6fffffff
#define elf_DT_LOPROC 0x70000000
#define elf_DT_HIPROC 0x7fffffff

// Values and macros for `elf_symbol.info`.
#define elf_ST_INFO(BIND, TYPE) (((BIND) << 4) | ((TYPE) & 0xf))
#define elf_ST_BIND(INFO) (((uint32_t)INFO) >> 4)
#define elf_ST_TYPE(INFO) (((uint32_t)INFO) & 0xf)
#define elf_STB_LOCAL 0
#define elf_STB_GLOBAL 1
#define elf_STB_WEAK 2
#define elf_STT_NOTYPE 0
#define elf_STT_OBJECT 1
#define elf_STT_FUNC 2
#define elf_STT_SECTION 3
#define elf_STT_FILE 4
#define elf_STT_COMMON 5
#define elf_STT_TLS 6

// Bit flags for `elf_programHeader.flags`.
#define elf_PF_R 0x4
#define elf_PF_W 0x2
#define elf_PF_X 0x1

// Values for `elf_sectionHeader.type`.
#define elf_SHT_NULL 0
#define elf_SHT_PROGBITS 1
#define elf_SHT_SYMTAB 2
#define elf_SHT_STRTAB 3
#define elf_SHT_RELA 4
#define elf_SHT_HASH 5
#define elf_SHT_DYNAMIC 6
#define elf_SHT_NOTE 7
#define elf_SHT_NOBITS 8
#define elf_SHT_REL 9
#define elf_SHT_SHLIB 10
#define elf_SHT_DYNSYM 11
#define elf_SHT_NUM 12
#define elf_SHT_LOPROC 0x70000000
#define elf_SHT_HIPROC 0x7fffffff
#define elf_SHT_LOUSER 0x80000000
#define elf_SHT_HIUSER 0xffffffff

// Bit flags for `elf_sectionHeader.flags`.
#define elf_SHF_WRITE 0x1
#define elf_SHF_ALLOC 0x2
#define elf_SHF_EXECINSTR 0x4
#define elf_SHF_RELA_LIVEPATCH 0x00100000
#define elf_SHF_RO_AFTER_INIT 0x00200000
#define elf_SHF_MASKPROC 0xf0000000

// Special indices for the section header table.
#define elf_SHN_UNDEF 0
#define elf_SHN_LORESERVE 0xff00
#define elf_SHN_LOPROC 0xff00
#define elf_SHN_HIPROC 0xff1f
#define elf_SHN_LIVEPATCH 0xff20
#define elf_SHN_ABS 0xfff1
#define elf_SHN_COMMON 0xfff2
#define elf_SHN_HIRESERVE 0xffff

struct elf_header {
    uint8_t magic[16];
    uint16_t type;
    uint16_t machine;
    uint32_t version;
    uint64_t entryAddress;
    uint64_t programHeadersOffset;
    uint64_t sectionHeadersOffset;
    uint32_t flags;
    uint16_t elfHeaderSize;
    uint16_t programHeaderSize;
    uint16_t programHeadersLength;
    uint16_t sectionHeaderSize;
    uint16_t sectionHeadersLength;
    uint16_t sectionNamesIndex; // Index into the section headers table.
};

struct elf_programHeader {
    uint32_t type;
    uint32_t flags;
    uint64_t fileOffset;
    uint64_t virtualAddress;
    uint64_t physicalAddress;
    uint64_t segmentFileSize;
    uint64_t segmentMemorySize;
    uint64_t segmentAlignment;
};

struct elf_sectionHeader {
    uint32_t nameIndex; // Index into the section names table.
    uint32_t type;
    uint64_t flags;
    uint64_t address;
    uint64_t fileOffset;
    uint64_t size;
    uint32_t link;
    uint32_t info;
    uint64_t addressAlignment;
    uint64_t entrySize;
};

struct elf_dynamicInfo {
    int64_t tag;
    uint64_t value;
};

struct elf_symbol {
    uint32_t nameIndex; // Index into string table.
    uint8_t info;
    uint8_t other;
    uint16_t sectionHeaderIndex;
    uint64_t value;
    uint64_t size;
};

struct elf_hashTable {
    uint32_t numBuckets;
    uint32_t numChains;
    uint32_t data[]; // `numBuckets + numChains` entries (in that order).
};