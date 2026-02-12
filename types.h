#ifndef CAN_IDPS_TYPES_H
#define CAN_IDPS_TYPES_H

#include <stdint.h>
#include <stdbool.h>

/* CAN 帧（简化，兼容 SocketCAN can_frame） */
#define CAN_MAX_DLEN 8

#define CAN_EFF_FLAG 0x80000000U
#define CAN_RTR_FLAG 0x40000000U
#define CAN_ERR_FLAG 0x20000000U
#define CAN_SFF_MASK 0x000007FFU
#define CAN_EFF_MASK 0x1FFFFFFFU

typedef struct can_idps_frame {
    uint32_t can_id;   /* ID + EFF/RTR/ERR flags */
    uint8_t  len;      /* 数据长度 0-8 */
    uint8_t  data[CAN_MAX_DLEN];
} can_frame_t;

/* 解码后的 CAN 包（供检测用） */
typedef struct can_packet {
    uint32_t can_id;      /* 原始 ID（含标志位） */
    uint32_t arb_id;      /* 仲裁 ID：11bit 或 29bit */
    bool     is_extended; /* 扩展帧 */
    bool     is_rtr;      /* 远程请求帧 */
    bool     is_error;    /* 错误帧 */
    uint8_t  len;
    const uint8_t *data;
} can_packet_t;

/* 规则 */
#define MAX_RULE_CONTENT 8
#define MAX_RULES        64

typedef struct can_rule {
    uint32_t can_id;       /* 匹配的 CAN ID（0=任意） */
    uint32_t can_id_mask;  /* ID 掩码（0=精确匹配 can_id） */
    uint8_t  content[MAX_RULE_CONTENT];
    uint8_t  content_len;
    char     msg[64];
    /* 速率限制：该 ID 每秒最大帧数，0=不限制 */
    uint32_t rate_limit;
} can_rule_t;

#endif /* CAN_IDPS_TYPES_H */
