#include "../../hc/hc.h"
#include "../../hc/libc.h"
#include "../../hc/libc/musl.c"
#include "../../hc/wrappers.c"
#include "../../hc/libhc/util.c"
#include "../../hc/libhc/debug.c"
#include "../../hc/libhc/drmKms.c"

static int32_t init_graphics(struct drmKms *graphics) {
    // Set up frame buffer.
    int32_t status = drmKms_init(graphics, "/dev/dri/card0");
    if (status < 0) {
        debug_printNum("Failed to set up frame buffer (", status, ")\n");
        return -1;
    }
    // Print all available modes.
    for (uint32_t i = 0; i < graphics->connector.count_modes; ++i) {
        debug_printStr("Mode \"", graphics->modeInfos[i].name, "\"\n", DRM_DISPLAY_MODE_LEN);
        debug_printNum("  Pixel clock: ", graphics->modeInfos[i].clock, " Khz\n");
        debug_printNum("  Width: ", graphics->modeInfos[i].hdisplay, " pixels\n");
        debug_printNum("  Height: ", graphics->modeInfos[i].vdisplay, " pixels\n");
        debug_printNum("  Hsync: start=", graphics->modeInfos[i].hsync_start, ", ");
        debug_printNum("end=", graphics->modeInfos[i].hsync_end, ", ");
        debug_printNum("total=", graphics->modeInfos[i].htotal, "\n");
        debug_printNum("  Vsync: start=", graphics->modeInfos[i].vsync_start, ", ");
        debug_printNum("end=", graphics->modeInfos[i].vsync_end, ", ");
        debug_printNum("total=", graphics->modeInfos[i].vtotal, "\n");
        debug_printNum("  Refresh rate: ", graphics->modeInfos[i].vrefresh, " hz\n\n");
    }

    // Do some drawing.
    memset(&graphics->frameBuffer[0], 127, (uint64_t)graphics->frameBufferSize);
    int32_t x = 100;
    int32_t y = 100;
    graphics->frameBuffer[(y * (int32_t)graphics->frameBufferInfo.width) + x] = 0x00FFFFFF;
    return 0;
}

static inline void deinit_graphics(struct drmKms *graphics) {
    drmKms_deinit(graphics);
}

int32_t main(hc_UNUSED int32_t argc, hc_UNUSED char **argv) {
    if (argc < 2) {
        static const char usageStart[] = "Usage: ";
        static const char usageArgs[] = " TTY_NUM\n";
        hc_write(STDOUT_FILENO, &usageStart, sizeof(usageStart) - 1);
        if (argv[0] != NULL) {
            int32_t len = 0;
            for (; argv[0][len] != '\0'; ++len);
            hc_write(STDOUT_FILENO, &argv[0][0], len);
        }
        hc_write(STDOUT_FILENO, &usageArgs, sizeof(usageArgs) - 1);
        return 0;
    }

    // Parse TTY_NUM argument.
    uint64_t ttyNumber;
    if (util_strToUint(argv[1], '\0', &ttyNumber) == 0 || ttyNumber < 1 || ttyNumber > 63) {
        static const char error[] = "Invalid TTY_NUM argument\n";
        hc_write(STDOUT_FILENO, &error, sizeof(error) - 1);
        return 1;
    }

    // Create path and open tty.
    char ttyPath[] = "/dev/tty\0";
    ttyPath[sizeof(ttyPath) - 2] = argv[1][0];
    if (ttyNumber > 9) {
       ttyPath[sizeof(ttyPath) - 1] = argv[1][1];
    }
    int32_t ttyFd = hc_openat(AT_FDCWD, &ttyPath, O_RDWR, 0);
    if (ttyFd < 0) {
        static const char error[] = "Failed to open tty\n";
        hc_write(STDOUT_FILENO, &error, sizeof(error) - 1);
        return -ttyFd;
    }

    // Create new session.
    int32_t status = hc_setsid();
    if (status < 0) {
        static const char error[] = "Failed to create new session (already session leader?)\n";
        hc_write(STDOUT_FILENO, &error, sizeof(error) - 1);
        return -status;
    }

    // Set the tty as our controlling terminal.
    status = hc_ioctl(ttyFd, TIOCSCTTY, 0);
    if (status < 0) {
        static const char error[] = "Failed to set controlling terminal\n";
        hc_write(STDOUT_FILENO, &error, sizeof(error) - 1);
        return -status;
    }

    // Set up signalfd for SIGUSR1 and SIGUSR2.
    uint64_t ttySignals = hc_SIGMASK(SIGUSR1) | hc_SIGMASK(SIGUSR2);
    status = hc_rt_sigprocmask(SIG_BLOCK, &ttySignals, NULL);
    if (status < 0) return 1;

    int32_t signalFd = hc_signalfd4(-1, &ttySignals, 0);
    if (signalFd < 0) {
        static const char error[] = "Failed to create signal fd\n";
        hc_write(STDOUT_FILENO, &error, sizeof(error) - 1);
        return -signalFd;
    }

    // Request SIGUSR1 and SIGUSR2 when our tty is entered and left.
    struct vt_mode vtMode = {
        .mode = VT_PROCESS,
        .acqsig = SIGUSR1,
        .relsig = SIGUSR2
    };
    status = hc_ioctl(ttyFd, VT_SETMODE, &vtMode);
    if (status < 0) {
        static const char error[] = "Failed to set tty mode\n";
        hc_write(STDOUT_FILENO, &error, sizeof(error) - 1);
        return -status;
    }

    // Check if our tty is already active.
    struct vt_stat vtState;
    status = hc_ioctl(ttyFd, VT_GETSTATE, &vtState);
    if (status < 0) {
        static const char error[] = "Failed to get tty state\n";
        hc_write(STDOUT_FILENO, &error, sizeof(error) - 1);
        return -status;
    }

    bool active = vtState.v_active == ttyNumber;

    struct drmKms graphics;
    if (active) {
        if (init_graphics(&graphics) < 0) return 1;
    }
    for (;;) {
        struct signalfd_siginfo info;
        int64_t numRead = hc_read(signalFd, &info, sizeof(info));
        if (numRead != sizeof(info)) {
            static const char error[] = "Failed to read from signal fd\n";
            hc_write(STDOUT_FILENO, &error, sizeof(error) - 1);
            return 1;
        }

        if (info.ssi_signo == SIGUSR1) {
            if (active) return 2;
            active = true;
            static const char message[] = "Acquired!\n";
            hc_write(STDOUT_FILENO, &message, sizeof(message) - 1);

            if (init_graphics(&graphics) < 0) return 1;
        } else if (info.ssi_signo == SIGUSR2) {
            if (!active) return 2;

            deinit_graphics(&graphics);

            status = hc_ioctl(ttyFd, VT_RELDISP, (void *)1);
            if (status < 0) {
                static const char error[] = "Failed to release display\n";
                hc_write(STDOUT_FILENO, &error, sizeof(error) - 1);
                return -status;
            }

            active = false;
            static const char message[] = "Released!\n";
            hc_write(STDOUT_FILENO, &message, sizeof(message) - 1);
        } else return 2;
    }

    return 0;
}