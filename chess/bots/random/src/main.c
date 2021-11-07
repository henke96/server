static struct client client;

int32_t main(int32_t argc, char **argv) {
    struct timespec currentTime;
    clock_gettime(CLOCK_MONOTONIC, &currentTime);
    unsigned int32_t seed = (unsigned int)(timespec_toNanoseconds(currentTime) % UINT_MAX);
    srand(seed);

    int32_t roomId = -1;
    if (argc > 1) roomId = atoi(argv[1]);

    client_create(&client, random_makeMove);
    int32_t status = client_run(&client, "127.0.0.1", 8089, roomId);
    printf("Status: %d\n", status);
    return 0;
}