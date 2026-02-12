#include "decoder.h"
#include "types.h"
#include <string.h>

void can_decode(const can_frame_t *frame, can_packet_t *out)
{
    out->can_id = frame->can_id;
    out->is_extended = (frame->can_id & CAN_EFF_FLAG) != 0;
    out->is_rtr = (frame->can_id & CAN_RTR_FLAG) != 0;
    out->is_error = (frame->can_id & CAN_ERR_FLAG) != 0;
    out->arb_id = frame->can_id & (out->is_extended ? CAN_EFF_MASK : CAN_SFF_MASK);
    out->len = frame->len > CAN_MAX_DLEN ? CAN_MAX_DLEN : frame->len;
    out->data = frame->data;
}
