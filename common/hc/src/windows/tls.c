// Thread local storage for Windows.
// See: http://www.nynaeve.net/?tag=tls

struct tls_imageTlsDirectory {
    uint64_t startAddressOfRawData;
    uint64_t endAddressOfRawData;
    uint64_t addressOfIndex;
    uint64_t addressOfCallBacks;
    uint32_t sizeOfZeroFill;
    uint32_t characteristics;
};

typedef void (*tls_imageTlsCallback)(void *dllHandle, uint32_t reason, void *reserved);

#pragma section(".CRT", read)
#pragma comment(linker, "/merge:.CRT=.rdata")
// To add a callback, use for example:
// hc_SECTION(".CRT$XLB") tls_imageTlsCallback __xl_b = &callbackFunction;
hc_SECTION(".CRT$XLA") tls_imageTlsCallback __xl_a = NULL;
hc_SECTION(".CRT$XLZ") tls_imageTlsCallback __xl_z = NULL;

hc_SECTION(".tls") uint64_t _tls_start;
hc_SECTION(".tls$ZZZ") uint64_t _tls_end;

// When building with `-ftls-model=local-exec` this doesn't actually get used by code generation.
// If using tls in a .dll or similar, change that option to `global-dynamic`.
uint32_t _tls_index;

const struct tls_imageTlsDirectory _tls_used = {
    .startAddressOfRawData = (uint64_t)&_tls_start,
    .endAddressOfRawData = (uint64_t)&_tls_end,
    .addressOfIndex = (uint64_t)&_tls_index,
    .addressOfCallBacks = (uint64_t)(&__xl_a + 1),
    .sizeOfZeroFill = 0,
    .characteristics = 0
};