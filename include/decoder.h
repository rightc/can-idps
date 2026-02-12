#ifndef CAN_IDPS_DECODER_H
#define CAN_IDPS_DECODER_H

#include "types.h"

/* 将 can_frame 解码为 can_packet */
void can_decode(const can_frame_t *frame, can_packet_t *out);

#endif /* CAN_IDPS_DECODER_H */
