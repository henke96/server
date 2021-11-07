static struct chess chess;

int32_t main(void) {
    int32_t status = chess_init(&chess);
    if (status < 0) {
        debug_printNum("Failed to initialize chess (", status, ")\n");
        return 1;
    }

    status = chess_run(&chess);
    if (status < 0) {
        debug_printNum("Chess ran into error (", status, ")\n");
        status = 1;
        goto cleanup_chess;
    }
    status = 0;
    cleanup_chess:
    chess_deinit(&chess);
    return status;
}
