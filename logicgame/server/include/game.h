struct game {
    struct server server;
    struct fileResponse response;
    struct client clients[server_MAX_CLIENTS];
};

static int32_t game_init(struct game *self);
static void game_deinit(struct game *self);

static int32_t game_run(struct game *self);
