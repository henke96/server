#define drmKms_MAX_CONNECTORS 16
#define drmKms_MAX_MODES 64

struct drmKms {
    int32_t cardFd;
    struct drm_mode_modeinfo modeInfos[drmKms_MAX_MODES];
    char __pad[4];
    struct drm_mode_get_connector connector;
    struct drm_mode_get_encoder encoder;
    struct drm_mode_fb_cmd frameBufferInfo;
    uint32_t *frameBuffer;
    int64_t frameBufferSize;
};

static int32_t drmKms_init(struct drmKms *self, const char *driCardPath) {
    self->cardFd = hc_openat(-1, driCardPath, O_RDWR, 0);
    if (self->cardFd < 0) return -1;

    // Get a list of connectors for this card.
    uint32_t connectorIds[drmKms_MAX_CONNECTORS];

    struct drm_mode_card_res cardResources = {
        .connector_id_ptr = &connectorIds[0],
        .count_connectors = drmKms_MAX_CONNECTORS
    };
    int32_t status = hc_ioctl(self->cardFd, DRM_IOCTL_MODE_GETRESOURCES, &cardResources);
    if (status < 0) {
        status = -2;
        goto cleanup_cardFd;
    }

    // Iterate over the connectors to find a suitable one.
    for (uint32_t i = 0; i < cardResources.count_connectors; ++i) {
        memset(&self->connector, 0, sizeof(self->connector));
        self->connector.connector_id = connectorIds[i];
        self->connector.modes_ptr = &self->modeInfos[0];
        self->connector.count_modes = drmKms_MAX_MODES;

        status = hc_ioctl(self->cardFd, DRM_IOCTL_MODE_GETCONNECTOR, &self->connector);
        if (status < 0) {
            status = -3;
            goto cleanup_cardFd;
        }

        if (
            self->connector.connection == DRM_CONNECTOR_STATUS_CONNECTED &&
            self->connector.count_modes > 0 &&
            self->connector.encoder_id != 0
        ) goto foundConnector;
    }
    // Did not find suitable connector.
    status = -4;
    goto cleanup_cardFd;

    foundConnector:
    // Kernel is silly and doesn't fill out any modes if it can't fit all of them..
    if (self->connector.count_modes > drmKms_MAX_MODES) {
        status = -5;
        goto cleanup_cardFd;
    }

    // Get encoder for connector.
    memset(&self->encoder, 0, sizeof(self->encoder));
    self->encoder.encoder_id = self->connector.encoder_id;
    status = hc_ioctl(self->cardFd, DRM_IOCTL_MODE_GETENCODER, &self->encoder);
    if (status < 0) {
        status = -6;
        goto cleanup_cardFd;
    }

    // Decide what mode to use..
    // TODO: Maybe this function should be split into two to give user chance to decide.
    //       Could also add support for more than 1 connector.
    int32_t selectedModeIndex = 0;

    // Create dumb buffer.
    struct drm_mode_create_dumb dumbBuffer = {
        .width = self->modeInfos[selectedModeIndex].hdisplay,
        .height = self->modeInfos[selectedModeIndex].vdisplay,
        .bpp = 32
    };
    status = hc_ioctl(self->cardFd, DRM_IOCTL_MODE_CREATE_DUMB, &dumbBuffer);
    if (status < 0) {
        status = -7;
        goto cleanup_cardFd;
    }
    self->frameBufferSize = (int64_t)dumbBuffer.size;  // The only dumbBuffer info that is not also in frameBufferInfo.

    // Create frame buffer based on the dumb buffer.
    memset(&self->frameBufferInfo, 0, sizeof(self->frameBufferInfo));
    self->frameBufferInfo.width = dumbBuffer.width;
    self->frameBufferInfo.height = dumbBuffer.height;
    self->frameBufferInfo.pitch = dumbBuffer.pitch;
    self->frameBufferInfo.bpp = dumbBuffer.bpp;
    self->frameBufferInfo.depth = 24;
    self->frameBufferInfo.handle = dumbBuffer.handle;
    status = hc_ioctl(self->cardFd, DRM_IOCTL_MODE_ADDFB, &self->frameBufferInfo);
    if (status < 0) {
        status = -8;
        goto cleanup_dumbBuffer;
    }
    // Prepare dumb buffer for mapping.
    struct drm_mode_map_dumb mapDumpBuffer = {
        .handle = dumbBuffer.handle
    };
    status = hc_ioctl(self->cardFd, DRM_IOCTL_MODE_MAP_DUMB, &mapDumpBuffer);
    if (status < 0) {
        status = -9;
        goto cleanup_frameBufferInfo;
    }
    // Map the buffer.
    self->frameBuffer = hc_mmap(NULL, self->frameBufferSize, PROT_READ | PROT_WRITE, MAP_SHARED, self->cardFd, (int64_t)mapDumpBuffer.offset);
    if ((int64_t)self->frameBuffer < 0) {
        status = -10;
        goto cleanup_frameBufferInfo;
    }

    // Set CRTC configuration.
    struct drm_mode_crtc setCrtc = {
        .set_connectors_ptr = &self->connector.connector_id,
        .count_connectors = 1,
        .crtc_id = self->encoder.crtc_id,
        .fb_id = self->frameBufferInfo.fb_id,
        .x = 0,
        .y = 0,
        .mode_valid = 1,
        .mode = self->modeInfos[selectedModeIndex]
    };
    status = hc_ioctl(self->cardFd, DRM_IOCTL_MODE_SETCRTC, &setCrtc);
    if (status < 0) {
        status = -11;
        goto cleanup_frameBuffer;
    }

    return 0;
    cleanup_frameBuffer:
    hc_munmap(self->frameBuffer, self->frameBufferSize);
    cleanup_frameBufferInfo:
    hc_ioctl(self->cardFd, DRM_IOCTL_MODE_RMFB, &self->frameBufferInfo.fb_id);
    cleanup_dumbBuffer:
    hc_ioctl(self->cardFd, DRM_IOCTL_MODE_DESTROY_DUMB, &(struct drm_mode_destroy_dumb) { .handle = dumbBuffer.handle });
    cleanup_cardFd:
    hc_close(self->cardFd);
    return status;
}

static inline void drmKms_deinit(struct drmKms *self) {
    hc_munmap(self->frameBuffer, self->frameBufferSize);
    hc_ioctl(self->cardFd, DRM_IOCTL_MODE_RMFB, &self->frameBufferInfo.fb_id);
    hc_ioctl(self->cardFd, DRM_IOCTL_MODE_DESTROY_DUMB, &(struct drm_mode_destroy_dumb) { .handle = self->frameBufferInfo.handle });
    hc_close(self->cardFd);
}