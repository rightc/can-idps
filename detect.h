#ifndef CAN_IDPS_DETECT_H
#define CAN_IDPS_DETECT_H

#include "types.h"

void can_detect_init(const char *rules_path);
void can_detect_cleanup(void);

/* 检测 CAN 帧，返回命中规则数 */
int can_detect_frame(const can_packet_t *pkt);

#endif /* CAN_IDPS_DETECT_H */
