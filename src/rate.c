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

#include "rate.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RATE_KEY(id) ((uint64_t)(id))

typedef struct {
    uint32_t can_id;
    uint32_t count;
    time_t   window_start;
} rate_entry_t;

static rate_entry_t g_table[RATE_TABLE_SIZE];
static uint32_t g_hash(uint32_t id)
{
    return (uint32_t)((id * 2654435761ULL) % RATE_TABLE_SIZE);
}

void can_rate_init(void)
{
    memset(g_table, 0, sizeof(g_table));
}

void can_rate_cleanup(void)
{
    memset(g_table, 0, sizeof(g_table));
}

bool can_rate_check(uint32_t can_id, uint32_t limit_per_sec)
{
    if (limit_per_sec == 0)
        return false;

    uint32_t idx = g_hash(can_id);
    time_t now = time(NULL);

    for (int i = 0; i < RATE_TABLE_SIZE; i++) {
        uint32_t j = (idx + i) % RATE_TABLE_SIZE;
        if (g_table[j].can_id == can_id) {
            if (now - g_table[j].window_start >= RATE_WINDOW_SEC) {
                g_table[j].count = 0;
                g_table[j].window_start = now;
            }
            g_table[j].count++;
            return g_table[j].count > limit_per_sec;
        }
        if (g_table[j].can_id == 0 || now - g_table[j].window_start >= RATE_WINDOW_SEC) {
            g_table[j].can_id = can_id;
            g_table[j].count = 1;
            g_table[j].window_start = now;
            return false;
        }
    }
    return false;
}
