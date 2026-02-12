#ifndef CAN_IDPS_RATE_H
#define CAN_IDPS_RATE_H

#include <stdbool.h>
#include <stdint.h>

#define RATE_WINDOW_SEC   1
#define RATE_TABLE_SIZE   256

void can_rate_init(void);
void can_rate_cleanup(void);

/* 检查是否超速，返回 true=超速需告警 */
bool can_rate_check(uint32_t can_id, uint32_t limit_per_sec);

#endif /* CAN_IDPS_RATE_H */
