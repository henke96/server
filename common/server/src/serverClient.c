static inline void serverClient_init(struct serverClient *self, int32_t index) {
    self->fd = -1;
    self->index = index;
}

static inline void serverClient_deinit(struct serverClient *self) {
    if (self->fd >= 0) {
        serverClient_close(self);
    }
}

static inline void serverClient_open(struct serverClient *self, int32_t fd) {
    self->fd = fd;
    self->receiveLength = 0;
    self->isWebsocket = false;
    self->clientDisconnectList = -1;
}

static inline void serverClient_close(struct serverClient *self) {
    sys_close(self->fd);
    self->fd = -1;
}
