/**
 * @file can.c
 */
#include "can.h"

void can_irq_handler()
{

}

void can_init(struct can *h, CAN_TypeDef *can)
{
    h->can = can;

	// enter init mode
    h->can->MCR |= CAN_MCR_INRQ;
	while ((h->can->MSR & CAN_MSR_INAK) != CAN_MSR_INAK)
		;

	// exit sleep
	h->can->MCR &= ~((uint32_t)CAN_MCR_SLEEP);

	//h->can->MCR &= ~CAN_MCR_SLEEP;

    h->can->MCR &= ~(CAN_MCR_TTCM | CAN_MCR_ABOM | CAN_MCR_AWUM );

	// 0.5 MBit/s
	// SILM: 0, LBKM: 0, t_RJW:2tq , t_BS1:11tq , t_BS3:4 , Prescaler: 1
	h->can->BTR = 0 << CAN_BTR_TS2_Pos
		| 1 << CAN_BTR_SJW_Pos
		| 3 << CAN_BTR_TS2_Pos
		| 10 << CAN_BTR_TS1_Pos
		| 0 << CAN_BTR_BRP_Pos;


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

void can_receive(struct can *h, uint16_t *can_id, uint8_t *msg[])
{

}
