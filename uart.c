#include "uart.h"

#include <stdbool.h>
#include <string.h>

struct uart {
    USART_TypeDef *uart;
    uint8_t tx_buffer[UART_TX_BUFFER_SIZE];
    uint8_t tx_next; // next byte to send, 0 after initialization
    uint8_t tx_end; // last byte to send, 0 after initialization
};

static struct uart s_uart[UART_CNT];
static int s_test;

void transmit_byte(struct uart *h)
{
    if (h->tx_next != h->tx_end) {
        h->uart->TDR = h->tx_buffer[h->tx_next];
        h->tx_next = (h->tx_next + 1) % UART_TX_BUFFER_SIZE;
    } else {
        h->uart->CR1 &= ~USART_CR1_TXEIE;
    }
}

void usart1_isr()
{
    struct uart *h = s_uart + UART1;

    if (USART1->ISR & USART_ISR_RXNE) {
        // reeceive
    } else if (USART1->ISR & USART_ISR_ORE) {
        // RX overrun error
    } else if (USART1->ISR & USART_ISR_TXE) {
        // send
        transmit_byte(h);
    }
}

void uart_init(enum UART id, uint32_t baud)
{
    int *a = &s_test;
    struct uart *h = s_uart + id;
    h->uart = USART1;

    h->uart->CR1 &= ~USART_CR1_UE; // start initialization

    h->uart->BRR = 8000000 / baud; // TODO parametrize sys clock value

    // 8N1, enable IRQs: RXNE/ORE, enable transmitter and receiver, enable
    // uart
    h->uart->CR1 |= USART_CR1_RXNEIE | USART_CR1_TE | USART_CR1_RE
            | USART_CR1_UE; // 8N1

    NVIC_EnableIRQ(USART1_IRQn);
}

void uart_send_data(enum UART id, const uint8_t *data, int len)
{
    struct uart *h = s_uart + id;
    const uint8_t *src_ptr = data;

    int remaining = len;

    while (remaining > 0) {
        NVIC_DisableIRQ(USART1_IRQn);

        // copy to TX buffer
        bool wrap = h->tx_next <= h->tx_end;
        if (wrap) {
            int bytes_to_end = UART_TX_BUFFER_SIZE - h->tx_end;
            // max_copy = bytes_to_end + h->tx_next;
            int num_copy
                    = (remaining <= bytes_to_end) ? remaining : bytes_to_end;
            if (num_copy != 0) {
                memcpy(h->tx_buffer + h->tx_end, src_ptr, num_copy);
                src_ptr += num_copy;
                remaining -= num_copy;
            }
            if (remaining > 0) {
                // needs wrap around
                num_copy = (h->tx_next < remaining) ? h->tx_next : remaining;
                if (num_copy != 0) {
                    memcpy(h->tx_buffer, src_ptr, num_copy);
                    h->tx_end = num_copy;
                    src_ptr += num_copy;
                    remaining -= num_copy;
                }
            } else {
                h->tx_end += num_copy;
            }
        } else {
            int space_left = h->tx_next - h->tx_end;
            unsigned int num_copy
                    = (unsigned int)((remaining <= space_left) ? remaining
                                                               : space_left);
            memcpy(h->tx_buffer + h->tx_end, src_ptr, num_copy);
            h->tx_end += num_copy;
            src_ptr += num_copy;
            remaining -= num_copy;
        }

        // enable transmit interrupt
        h->uart->CR1 |= USART_CR1_TXEIE;

        NVIC_EnableIRQ(USART1_IRQn);

        // blocking wait if buffer is full
        if (remaining > 0) {
            while (1) {
                int cnt = 100;
                while (cnt--) {
                    __ASM volatile ("nop" : : : );
                }
                NVIC_DisableIRQ(USART1_IRQn);

                if (h->uart->ISR & USART_ISR_TXE) {
                    NVIC_EnableIRQ(USART1_IRQn);
                    continue;
                }
                NVIC_EnableIRQ(USART1_IRQn);
            }
        }
    }
}
