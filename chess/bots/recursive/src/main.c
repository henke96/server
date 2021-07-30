#include "client.h"
#include "recursive.h"
#include "timespec.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

static struct client main_client;

int main(int argc, char **argv) {
    struct timespec currentTime;
    clock_gettime(CLOCK_MONOTONIC, &currentTime);
    unsigned int seed = timespec_toNanoseconds(currentTime) % UINT_MAX;
    srand(seed);

    int32_t roomId = -1;
    if (argc > 1) roomId = atoi(argv[1]);

    client_create(&main_client, recursive_makeMove);
    int status = client_run(&main_client, "127.0.0.1", 8089, roomId);
    printf("Status: %d\n", status);
    return 0;
}