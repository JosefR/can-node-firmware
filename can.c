/**
 * @file can.c
 */
#include "can.h"

void can_init(struct can *h, CAN_TypeDef *can)
{
    h->can = can;

    // exit sleep
    h->can->MCR &= ~((uint32_t)CAN_MCR_SLEEP);

    // init mode
    h->can->MCR |= CAN_MCR_INRQ;
    while ((h->can->MSR & CAN_MSR_INAK) != CAN_MSR_INAK);

    //h->can->MCR &= ~CAN_MCR_SLEEP;

    h->can->MCR &= ~(CAN_MCR_TTCM | CAN_MCR_ABOM | CAN_MCR_AWUM );

    // loopback mode, timing
    h->can->BTR &= ~(CAN_BTR_SILM | CAN_BTR_LBKM | CAN_BTR_SJW |
        CAN_BTR_TS2 | CAN_BTR_TS1 | CAN_BTR_BRP);
    h->can->BTR |= CAN_BTR_LBKM | 0 << CAN_BTR_TS2_Pos |
        0 << CAN_BTR_TS1_Pos | (15 & CAN_BTR_BRP);

    // leave init mode
    h->can->MCR &= ~((uint32_t)CAN_MCR_INRQ);
    while ((h->can->MSR & CAN_MSR_INAK) == CAN_MSR_INAK) {
    }
}

void can_send(struct can *h, uint16_t can_id, uint8_t msg[8])
{
    if ((h->can->TSR & CAN_TSR_TME0) == CAN_TSR_TME0) {
        h->can->sTxMailBox[0].TDTR = 8; // length: 8 byte
        h->can->sTxMailBox[0].TDLR = msg[3] << 24 | msg[2] << 16 | msg[1] << 8 | msg[0];
        h->can->sTxMailBox[0].TDHR = msg[7] << 24 | msg[6] << 16 | msg[5] << 8 | msg[4];
        h->can->sTxMailBox[0].TIR = (uint32_t)(can_id << 21 | CAN_TI0R_TXRQ); // transmit mailbox request
    }
}
