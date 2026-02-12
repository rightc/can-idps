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

#include "output.h"
#include <stdio.h>
#include <string.h>

void can_output_alert(const char *msg, const can_packet_t *pkt)
{
    char safe_msg[64];
    size_t i;
    for (i = 0; i < sizeof(safe_msg) - 1 && msg[i]; i++)
        safe_msg[i] = (msg[i] == '"') ? '\'' : msg[i];
    safe_msg[i] = '\0';

    printf("{\"msg\":\"%s\",\"can_id\":\"0x%03X\",\"ext\":%s,\"rtr\":%s,\"len\":%u,\"data\":\"",
           safe_msg, pkt->arb_id,
           pkt->is_extended ? "true" : "false",
           pkt->is_rtr ? "true" : "false",
           pkt->len);
    for (uint32_t j = 0; j < pkt->len && j < 8; j++)
        printf("%02X", pkt->data[j]);
    printf("\"}\n");
    fflush(stdout);
}
