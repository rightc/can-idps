#include "capture.h"
#include "decoder.h"
#include "detect.h"
#include "output.h"
#include "rate.h"
#include "types.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static volatile sig_atomic_t g_running = 1;

static void frame_handler(const can_frame_t *frame, void *ctx)
{
    (void)ctx;
    can_packet_t pkt;
    can_decode(frame, &pkt);
    can_detect_frame(&pkt);
}

int main(int argc, char **argv)
{
    const char *iface = "can0";
    const char *rules = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            iface = argv[++i];
        } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            rules = argv[++i];
        } else if (strcmp(argv[i], "-h") == 0) {
            fprintf(stderr,
                    "Usage: %s [-i iface] [-r rules]\n"
                    "  CAN IDPS: Intrusion detection for CAN bus (SocketCAN)\n"
                    "  Example: %s -i can0 -r rules/can.rules\n",
                    argv[0], argv[0]);
            return 0;
        }
    }

    can_rate_init();
    can_detect_init(rules);

    if (can_capture_init(iface) != 0) {
        fprintf(stderr, "capture init failed\n");
        can_detect_cleanup();
        can_rate_cleanup();
        return 1;
    }

    fprintf(stderr, "can-idps: listening on %s (SocketCAN)\n", iface);
    can_capture_run(frame_handler, NULL);
    can_capture_free();

    can_detect_cleanup();
    can_rate_cleanup();
    return 0;
}
