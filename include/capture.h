#ifndef CAN_IDPS_CAPTURE_H
#define CAN_IDPS_CAPTURE_H

#include "types.h"

typedef void (*can_frame_cb_t)(const can_frame_t *frame, void *ctx);

/* 初始化 SocketCAN 抓包（Linux can0, vcan0 等） */
int can_capture_init(const char *iface);

/* 运行抓包循环 */
void can_capture_run(can_frame_cb_t cb, void *ctx);

/* 释放 */
void can_capture_free(void);

#endif /* CAN_IDPS_CAPTURE_H */
