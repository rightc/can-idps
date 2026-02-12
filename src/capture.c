/* MIT License
*
*Copyright (c) 2025 right_c
*
*Permission is hereby granted, free of charge, to any person obtaining a copy
*of this software and associated documentation files (the "Software"), to deal
*in the Software without restriction, including without limitation the rights
*to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*copies of the Software, and to permit persons to whom the Software is
*furnished to do so, subject to the following conditions:
*
*The above copyright notice and this permission notice shall be included in all
*copies or substantial portions of the Software.
*
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*SOFTWARE.
*/   

#include "capture.h"
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>

#include <linux/can.h>
#include <linux/can/raw.h>

static int g_sock = -1;
static volatile sig_atomic_t g_running = 1;

static void sig_handler(int sig)
{
    (void)sig;
    g_running = 0;
}

int can_capture_init(const char *iface)
{
    int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) {
        perror("socket(PF_CAN, SOCK_RAW)");
        return -1;
    }

    struct sockaddr_can addr = {0};
    struct ifreq ifr = {0};

    strncpy(ifr.ifr_name, iface, IFNAMSIZ - 1);
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        fprintf(stderr, "CAN interface '%s' not found\n", iface);
        close(sock);
        return -1;
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind(CAN)");
        close(sock);
        return -1;
    }

    int flags = fcntl(sock, F_GETFL);
    if (flags >= 0)
        fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    g_sock = sock;
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    return 0;
}

void can_capture_run(can_frame_cb_t cb, void *ctx)
{
    struct can_frame frame;
    struct pollfd pfd = {.fd = g_sock, .events = POLLIN};

    while (g_running) {
        int r = poll(&pfd, 1, 200);
        if (r < 0) {
            if (errno == EINTR)
                continue;
            perror("poll");
            break;
        }
        if (r == 0)
            continue;

        ssize_t n = read(g_sock, &frame, sizeof(frame));
        if (n != sizeof(frame))
            continue;

        can_frame_t cf = {
            .can_id = frame.can_id,
            .len = frame.len,
        };
        memcpy(cf.data, frame.data, frame.len > CAN_MAX_DLEN ? CAN_MAX_DLEN : frame.len);
        cb(&cf, ctx);
    }
}

void can_capture_free(void)
{
    if (g_sock >= 0) {
        close(g_sock);
        g_sock = -1;
    }
}
