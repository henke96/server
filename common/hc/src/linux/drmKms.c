#define drmKms_MAX_CONNECTORS 16
#define drmKms_MAX_MODES 64

struct drmKms {
    int32_t cardFd;
    struct drm_mode_fb_cmd frameBufferInfo;
    struct drm_mode_get_connector connector;
    struct drm_mode_modeinfo modeInfos[drmKms_MAX_MODES];
    uint32_t *frameBuffer; // If not NULL, a framebuffer is setup.
    int64_t frameBufferSize;
    uint32_t crtcId;
    int32_t __pad;
};

static int32_t drmKms_init(struct drmKms *self, const char *driCardPath) {
    self->cardFd = sys_openat(-1, driCardPath, O_RDWR, 0);
    if (self->cardFd < 0) return -1;

    // Get a list of connectors and one crtc for this card.
    uint32_t connectorIds[drmKms_MAX_CONNECTORS];

    struct drm_mode_card_res cardResources = {
        .connector_id_ptr = &connectorIds[0],
        .count_connectors = drmKms_MAX_CONNECTORS,
        .crtc_id_ptr = &self->crtcId,
        .count_crtcs = 1
    };
    int32_t status = sys_ioctl(self->cardFd, DRM_IOCTL_MODE_GETRESOURCES, &cardResources);
    if (status < 0) {
        status = -2;
        goto cleanup_cardFd;
    }

    if (cardResources.count_crtcs < 1) {
        status = -3;
        goto cleanup_cardFd;
    }

    // Iterate over the connectors to find a suitable one.
    for (uint32_t i = 0; i < cardResources.count_connectors; ++i) {
        hc_MEMSET(&self->connector, 0, sizeof(self->connector));
        self->connector.connector_id = connectorIds[i];
        self->connector.modes_ptr = &self->modeInfos[0];
        self->connector.count_modes = drmKms_MAX_MODES;

        status = sys_ioctl(self->cardFd, DRM_IOCTL_MODE_GETCONNECTOR, &self->connector);
        if (status < 0) {
            status = -4;
            goto cleanup_cardFd;
        }

        if (
            self->connector.connection == DRM_CONNECTOR_STATUS_CONNECTED &&
            self->connector.count_modes > 0
        ) goto foundConnector;
    }
    // Did not find suitable connector.
    status = -5;
    goto cleanup_cardFd;

    foundConnector:
    // Kernel is silly and doesn't fill out any modes if it can't fit all of them..
    if (self->connector.count_modes > drmKms_MAX_MODES) {
        status = -6;
        goto cleanup_cardFd;
    }

    self->frameBuffer = NULL; // No framebuffer setup yet.
    return 0;

    cleanup_cardFd:
    sys_close(self->cardFd);
    return status;
}

static int32_t drmKms_setupFb(struct drmKms *self, int32_t modeIndex) {
    // Create dumb buffer.
    struct drm_mode_create_dumb dumbBuffer = {
        .width = self->modeInfos[modeIndex].hdisplay,
        .height = self->modeInfos[modeIndex].vdisplay,
        .bpp = 32
    };
    int32_t status = sys_ioctl(self->cardFd, DRM_IOCTL_MODE_CREATE_DUMB, &dumbBuffer);
    if (status < 0) return -1;
    self->frameBufferSize = (int64_t)dumbBuffer.size;

    // Create frame buffer based on the dumb buffer.
    hc_MEMSET(&self->frameBufferInfo, 0, sizeof(self->frameBufferInfo));
    self->frameBufferInfo.width = dumbBuffer.width;
    self->frameBufferInfo.height = dumbBuffer.height;
    self->frameBufferInfo.pitch = dumbBuffer.pitch;
    self->frameBufferInfo.bpp = dumbBuffer.bpp;
    self->frameBufferInfo.depth = 24;
    self->frameBufferInfo.handle = dumbBuffer.handle;
    status = sys_ioctl(self->cardFd, DRM_IOCTL_MODE_ADDFB, &self->frameBufferInfo);
    if (status < 0) {
        status = -2;
        goto cleanup_dumbBuffer;
    }

    // Prepare dumb buffer for mapping.
    struct drm_mode_map_dumb mapDumpBuffer = {
        .handle = dumbBuffer.handle
    };
    status = sys_ioctl(self->cardFd, DRM_IOCTL_MODE_MAP_DUMB, &mapDumpBuffer);
    if (status < 0) {
        status = -3;
        goto cleanup_frameBufferInfo;
    }

    // Map the buffer.
    self->frameBuffer = sys_mmap(NULL, self->frameBufferSize, PROT_READ | PROT_WRITE, MAP_SHARED, self->cardFd, (int64_t)mapDumpBuffer.offset);
    if ((int64_t)self->frameBuffer < 0) {
        status = -4;
        goto cleanup_frameBufferInfo;
    }

    // Set CRTC configuration.
    struct drm_mode_crtc setCrtc = {
        .set_connectors_ptr = &self->connector.connector_id,
        .count_connectors = 1,
        .crtc_id = self->crtcId,
        .fb_id = self->frameBufferInfo.fb_id,
        .mode_valid = 1,
        .mode = self->modeInfos[modeIndex]
    };
    status = sys_ioctl(self->cardFd, DRM_IOCTL_MODE_SETCRTC, &setCrtc);
    if (status < 0) {
        status = -5;
        goto cleanup_frameBuffer;
    }
    return 0;

    cleanup_frameBuffer:
    sys_munmap(self->frameBuffer, self->frameBufferSize);
    self->frameBuffer = NULL;
    cleanup_frameBufferInfo:
    sys_ioctl(self->cardFd, DRM_IOCTL_MODE_RMFB, &self->frameBufferInfo.fb_id);
    cleanup_dumbBuffer:
    sys_ioctl(self->cardFd, DRM_IOCTL_MODE_DESTROY_DUMB, &(struct drm_mode_destroy_dumb) { .handle = dumbBuffer.handle });
    return status;
}

// Call after updating framebuffer to make sure it gets displayed on screen.
static inline void drmKms_markFbDirty(struct drmKms *self) {
    struct drm_mode_fb_dirty_cmd fbDirty = { .fb_id = self->frameBufferInfo.fb_id };
    sys_ioctl(self->cardFd, DRM_IOCTL_MODE_DIRTYFB, &fbDirty);
}

static inline void drmKms_deinit(struct drmKms *self) {
    if (self->frameBuffer != NULL) {
        sys_munmap(self->frameBuffer, self->frameBufferSize);
        sys_ioctl(self->cardFd, DRM_IOCTL_MODE_RMFB, &self->frameBufferInfo.fb_id);
        sys_ioctl(self->cardFd, DRM_IOCTL_MODE_DESTROY_DUMB, &(struct drm_mode_destroy_dumb) { .handle = self->frameBufferInfo.handle });
    }
    sys_close(self->cardFd);
}