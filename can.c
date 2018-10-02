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
		| 0 << CAN_BTR_BRP_Pos
		| CAN_BTR_LBKM;


    // leave init mode
	h->can->MCR &= ~((uint32_t)CAN_MCR_INRQ);
    while ((h->can->MSR & CAN_MSR_INAK) == CAN_MSR_INAK) {
	}

	// enter filter initialization
	h->can->FMR |= CAN_FMR_FINIT;

	h->can->FA1R &= ~CAN_FA1R_FACT0; // Filter 0 dactivate
	h->can->FM1R |= CAN_FM1R_FBM0; // Filter 0: identifier mask mode
	h->can->FS1R |= CAN_FS1R_FSC0; // Filter 0: 32-bit scale
	h->can->FFA1R &= ~CAN_FA1R_FACT0; // assign FIFO 0

	h->can->sFilterRegister[0].FR1 =  (0b10101010101 << 5 << 16) | 0;


	h->can->FA1R |= CAN_FA1R_FACT0; // Filter 0 activate



	h->can->FMR &= ~CAN_FMR_FINIT;

	//	NVIC_EnableIRQ(CEC_CAN_IRQn);
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

bool can_receive(struct can *h, uint16_t *can_id, uint8_t *msg[])
{
	uint32_t msg_l;
	uint32_t msg_h;
	/* check if a message is filtered and received by FIFO 0 */
	if ((h->can->RF0R & CAN_RF0R_FMP0) != 0) {
		msg_l = h->can->sFIFOMailBox[0].RDLR; /* read data */
		msg_h = h->can->sFIFOMailBox[0].RDHR; /* read data */
		h->can->RF0R |= CAN_RF0R_RFOM0; /* release FIFO */

		msg[0] = msg_l & 0xff;
		msg[1] = (msg_l  >> 8) &  0xff;
		msg[2] = (msg_l  >> 16) &  0xff;
		msg[3] = (msg_l  >> 24) &  0xff;
		msg[4] = msg_h & 0xff;
		msg[5] = (msg_h  >> 8) &  0xff;
		msg[6] = (msg_h  >> 16) &  0xff;
		msg[7] = (msg_h  >> 24) &  0xff;

		return true;
	}

	return false;
}

