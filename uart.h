#ifndef CANNODE_UART_H
#define CANNODE_UART_H

#include "stm32f072xb.h"

#define UART_TX_BUFFER_SIZE 64

enum UART {
    UART1,
    UART2,
    UART3,
    UART4,
    UART_CNT
};

struct uart;

void uart_init(enum UART id, uint32_t baud);

void uart_send_data(enum UART id, const uint8_t *data, int len);

#endif // CANNODE_UART_H
