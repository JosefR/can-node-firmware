/**
 * @file can.h
 */
#ifndef CANNODE_CAN_H
#define CANNODE_CAN_H

#include "stm32f072xb.h"

struct can {
    CAN_TypeDef *can;
};

void can_init(struct can *h, CAN_TypeDef *can);

void can_send(struct can *h, uint16_t can_id, uint8_t msg[8]);

void can_receive(struct can *h, uint16_t *can_id, uint8_t *msg[8]);

#endif // CANNODE_CAN_H
