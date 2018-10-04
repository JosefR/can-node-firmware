/**
 * @file can.c
 */
#include "can.h"

static struct can {
    can_rx_callback rx_cb;
} s_can;

void can_isr()
{
    uint16_t can_id;
    uint8_t can_msg[8];

    uint32_t msg_l;
    uint32_t msg_h;
    /* check if a message is filtered and received by FIFO 0 */
    if (CAN->RF0R & CAN_RF0R_FMP0) {

        can_id = CAN->sFIFOMailBox[0].RIR >> CAN_RI0R_STID_Pos;

        msg_l = CAN->sFIFOMailBox[0].RDLR; /* read data */
        msg_h = CAN->sFIFOMailBox[0].RDHR; /* read data */

        // TODO: execute callback
        if (s_can.rx_cb) {
            // TODO: length
            s_can.rx_cb(can_id, msg_l, msg_h, 8);
        }

        CAN->RF0R |= CAN_RF0R_RFOM0; /* release FIFO */

    }
}

void can_init(can_rx_callback cb)
{
    s_can.rx_cb = cb;

    // enter init mode
    CAN->MCR |= CAN_MCR_INRQ;
    while ((CAN->MSR & CAN_MSR_INAK) != CAN_MSR_INAK) {
    }

    // exit sleep
    CAN->MCR &= ~((uint32_t)CAN_MCR_SLEEP);

    //CAN->MCR &= ~CAN_MCR_SLEEP;

    CAN->MCR &= ~(CAN_MCR_TTCM | CAN_MCR_ABOM | CAN_MCR_AWUM );

	// 0.5 MBit/s
	// SILM: 0, LBKM: 0, t_RJW:2tq , t_BS1:11tq , t_BS3:4 , Prescaler: 1
    CAN->BTR = 0 << CAN_BTR_TS2_Pos
        | 1 << CAN_BTR_SJW_Pos
        | 3 << CAN_BTR_TS2_Pos
        | 10 << CAN_BTR_TS1_Pos
        | 0 << CAN_BTR_BRP_Pos
        | CAN_BTR_LBKM;


    // leave init mode
    CAN->MCR &= ~((uint32_t)CAN_MCR_INRQ);
    while ((CAN->MSR & CAN_MSR_INAK) == CAN_MSR_INAK) {
	}

    // enter filter initialization
    CAN->FMR |= CAN_FMR_FINIT;

    CAN->FA1R &= ~CAN_FA1R_FACT0; // Filter 0 dactivate
    CAN->FM1R |= CAN_FM1R_FBM0; // Filter 0: identifier mask mode
    CAN->FS1R |= CAN_FS1R_FSC0; // Filter 0: 32-bit scale
    CAN->FFA1R &= ~CAN_FA1R_FACT0; // assign FIFO 0

    CAN->sFilterRegister[0].FR1 =  (0b10101010101 << 5 << 16) | 0;


    CAN->FA1R |= CAN_FA1R_FACT0; // Filter 0 activate



    CAN->FMR &= ~CAN_FMR_FINIT;

    CAN->IER |= CAN_IER_FMPIE0;

    NVIC_EnableIRQ(CEC_CAN_IRQn);
}

void can_send(uint16_t can_id, uint8_t msg[8])
{
    if ((CAN->TSR & CAN_TSR_TME0) == CAN_TSR_TME0) {
        CAN->sTxMailBox[0].TIR &= CAN_TI0R_TXRQ; // clear
        CAN->sTxMailBox[0].TDTR = (CAN->sTxMailBox[0].TDTR & ~(CAN_TDT1R_DLC)) | 8; // length: 8 byte
        CAN->sTxMailBox[0].TDLR = msg[3] << 24 | msg[2] << 16 | msg[1] << 8 | msg[0];
        CAN->sTxMailBox[0].TDHR = msg[7] << 24 | msg[6] << 16 | msg[5] << 8 | msg[4];
        CAN->sTxMailBox[0].TIR = (uint32_t)(can_id << CAN_TI0R_STID_Pos | CAN_TI0R_TXRQ); // transmit mailbox request
    }
}

bool can_receive(uint16_t *can_id, uint8_t *msg[])
{
    uint32_t msg_l;
    uint32_t msg_h;
    /* check if a message is filtered and received by FIFO 0 */
    if ((CAN->RF0R & CAN_RF0R_FMP0) != 0) {
        msg_l = CAN->sFIFOMailBox[0].RDLR; /* read data */
        msg_h = CAN->sFIFOMailBox[0].RDHR; /* read data */
        CAN->RF0R |= CAN_RF0R_RFOM0; /* release FIFO */

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
