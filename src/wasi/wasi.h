_Static_assert(hc_ILP32, "Pointers not 32 bit");

struct ciovec {
    const uint8_t *buffer;
    uint32_t bufferLength;
};

hc_WASM_IMPORT("wasi_unstable", "fd_write")
uint16_t fd_write(int32_t fd, const struct ciovec *iovs, uint32_t iovsLen, uint32_t *ret);

hc_WASM_IMPORT("wasi_unstable", "proc_exit")
void noreturn proc_exit(uint32_t exitCode);