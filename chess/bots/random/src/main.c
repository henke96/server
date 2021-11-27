static struct client client;

int32_t main(int32_t argc, char **argv) {
    int32_t roomId = -1;
    if (argc > 1) {
        roomId = 0;
        for (char *s = argv[1]; *s != '\0'; ++s) {
            if (*s < '0' || *s > '9') return 1;
            roomId = 10 * roomId + (*s - '\0');
        }
    }

    client_create(&client, random_makeMove);
    uint8_t address[] = {
        127, 0, 0, 1
    };
    int32_t status = client_run(&client, &address[0], 8089, roomId);
    debug_printNum("Status: ", status, "\n");
    return 0;
}