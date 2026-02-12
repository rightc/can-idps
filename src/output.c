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
