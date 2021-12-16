struct game;
struct serverClient;

struct client {
    struct serverClient *serverClient;
};

static void client_create(struct client *self, struct serverClient *serverClient);
