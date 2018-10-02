/**
 * @file can.h
 */
#ifndef CANNODE_CAN_H
#define CANNODE_CAN_H

#include <stdbool.h>
#include "stm32f072xb.h"

typedef  void (*can_rx_callback)(uint16_t, uint32_t, uint32_t, uint8_t);

struct can;

void can_init(can_rx_callback cb);

void can_send(uint16_t can_id, uint8_t msg[8]);

bool can_receive(uint16_t* can_id, uint8_t* msg[8]);

#endif  // CANNODE_CAN_H
