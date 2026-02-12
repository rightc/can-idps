#include "detect.h"
#include "output.h"
#include "rate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static can_rule_t g_rules[MAX_RULES];
static int g_rule_count;

static void add_default_rules(void)
{
    can_rule_t r = {0};

    /* 检测错误帧 */
    r.can_id = CAN_ERR_FLAG;
    r.can_id_mask = CAN_ERR_FLAG;
    strncpy(r.msg, "CAN error frame", sizeof(r.msg) - 1);
    if (g_rule_count < MAX_RULES)
        g_rules[g_rule_count++] = r;

    /* 常见敏感 CAN ID 示例（OBD-II、车身控制等，可按需配置） */
    r = (can_rule_t){0};
    r.can_id = 0x7DF;
    r.can_id_mask = CAN_SFF_MASK;
    strncpy(r.msg, "CAN OBD-II broadcast request", sizeof(r.msg) - 1);
    if (g_rule_count < MAX_RULES)
        g_rules[g_rule_count++] = r;

    /* 速率限制示例：某 ID 每秒超过 100 帧告警 */
    r = (can_rule_t){0};
    r.can_id = 0;
    r.can_id_mask = 0;
    r.rate_limit = 100;
    strncpy(r.msg, "CAN flood (generic rate limit)", sizeof(r.msg) - 1);
    if (g_rule_count < MAX_RULES)
        g_rules[g_rule_count++] = r;
}

static bool match_id(uint32_t pkt_id, uint32_t rule_id, uint32_t rule_mask)
{
    if (rule_id == 0 && rule_mask == 0)
        return true;
    uint32_t m = (rule_mask != 0) ? rule_mask : 0x1FFFFFFFU;
    return (pkt_id & m) == (rule_id & m);
}

static bool match_content(const uint8_t *data, uint8_t len, const can_rule_t *r)
{
    if (r->content_len == 0)
        return true;
    if (len < r->content_len)
        return false;
    for (uint32_t i = 0; i + r->content_len <= len; i++) {
        if (memcmp(data + i, r->content, r->content_len) == 0)
            return true;
    }
    return false;
}

void can_detect_init(const char *rules_path)
{
    g_rule_count = 0;
    add_default_rules();

    if (!rules_path)
        return;

    FILE *f = fopen(rules_path, "r");
    if (!f)
        return;

    char line[256];
    while (fgets(line, sizeof(line), f) && g_rule_count < MAX_RULES) {
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (*p == '#' || *p == '\n' || *p == '\0')
            continue;

        can_rule_t r = {0};
        unsigned int id, mask = 0;
        if (sscanf(p, "alert 0x%x 0x%x", &id, &mask) >= 1) {
            r.can_id = id;
            r.can_id_mask = (mask != 0) ? mask : CAN_SFF_MASK;

            char *msg = strstr(p, "msg:");
            if (msg) {
                msg += 4;
                while (*msg == '"') msg++;
                char *end = strchr(msg, '"');
                if (end) {
                    size_t n = end - msg;
                    if (n >= sizeof(r.msg)) n = sizeof(r.msg) - 1;
                    memcpy(r.msg, msg, n);
                    r.msg[n] = '\0';
                }
            }
            char *rate = strstr(p, "rate:");
            if (rate && sscanf(rate + 5, "%u", &r.rate_limit) == 1)
                ;
            g_rules[g_rule_count++] = r;
        }
    }
    fclose(f);
}

void can_detect_cleanup(void)
{
    g_rule_count = 0;
}

int can_detect_frame(const can_packet_t *pkt)
{
    int hit = 0;
    for (int i = 0; i < g_rule_count; i++) {
        const can_rule_t *r = &g_rules[i];
        if (!match_id(pkt->can_id, r->can_id, r->can_id_mask))
            continue;
        if (r->rate_limit != 0) {
            if (can_rate_check(pkt->can_id, r->rate_limit)) {
                can_output_alert(r->msg, pkt);
                hit++;
            }
            continue;
        }
        if (!match_content(pkt->data, pkt->len, r))
            continue;

        can_output_alert(r->msg, pkt);
        hit++;
    }
    return hit;
}
