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
