hc_UNUSED
static inline char **util_getEnvp(int32_t argc, char **argv) {
    return argv + argc + 1;
}

hc_UNUSED
static uint64_t *util_getAuxv(char **envp) {
    for (; *envp != NULL; ++envp);
    return (uint64_t *)(envp + 1);
}