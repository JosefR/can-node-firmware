#include <stdint.h>
#include <stdbool.h>
#include <stm32f072xb.h>
#include "display.h"
#include "hdc1080.h"
#include "can.h"
#include "systick.h"
#include "scheduler.h"
#include "err.h"

void clock_init()
{
    // just use the default internal RC oscillator for now

    // enable GPIO peripheral clock for port A, B and C
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;

    // enable UART1 clock
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // enable I2C2 Clock
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    // enable CAN clock
    RCC->APB1ENR |= RCC_APB1ENR_CANEN;

    systick_init(8000);
}

void gpio_init()
{
    uint32_t tmp;

    // set A0 amd A10 to alternate function
    tmp = GPIOA->MODER;
    tmp |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1;
    GPIOA->MODER = tmp;

    // A9: USART1_TX, A10: USART1_RX
    tmp = GPIOA->AFR[1];
    tmp |= 0x1 << GPIO_AFRH_AFRH1_Pos | 0x1 << GPIO_AFRH_AFRH2_Pos;
    GPIOA->AFR[1] = tmp;

    // set B14 and B15 to output, B8, B9, B10 and B11 to alternate function
    tmp = GPIOB->MODER;
    tmp |=  GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0
        | GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1
        | GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;
    GPIOB->MODER = tmp;

    // set alternate functions for GPIO B: Pin 8: CAN_RX, Pin 9: CAN_TX,
    // Pin 10: I2C2_SDL, Pin 11: I2C2_SDA
    tmp = GPIOB->AFR[1];
    tmp |= 0x4 << GPIO_AFRH_AFRH0_Pos | 0x4 << GPIO_AFRH_AFRH1_Pos
        | 0x1 << GPIO_AFRH_AFRH2_Pos | 0x1 << GPIO_AFRH_AFRH3_Pos;
    GPIOB->AFR[1] = tmp;

    tmp = GPIOB->OSPEEDR;
    tmp |= 0x3 << GPIO_OSPEEDR_OSPEEDR8_Pos | 0x3 << GPIO_OSPEEDR_OSPEEDR9_Pos;
    GPIOB->OSPEEDR = tmp;

    // set C0 - C12 to output
    GPIOC->MODER |=  GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 |
        GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 |
        GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 |
        GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER10_0 |
        GPIO_MODER_MODER11_0 | GPIO_MODER_MODER12_0;
}

void usart1_handler()
{

}

void uart_init()
{
    USART1->BRR = 80000 / 96;
    USART1->CR1 = USART_CR1_TE | USART_CR1_UE;
}

void uart_send()
{
    uint32_t tmp = USART1->ISR;

    USART1->TDR = 'm';


//    if ((USART1->ISR & USART_ISR_TC) == USART_ISR_TC)
//    {
//    if (send == sizeof(stringtosend))
//    {
//    send=0;
//    USART1->ICR |= USART_ICR_TCCF; /* Clear transfer complete flag */
//    }
//    else
//    {
//    /* clear transfer complete flag and fill TDR with a new char */
//    USART1->TDR = stringtosend[send++];
//    }
//    }
}

void read_sensors()
{
    // TODO
}

int main()
{
    struct can can_h;
    struct i2c i2c_h;
    struct hdc1080 hdc1080_h;
    struct display dspl_h = {{0}, 0};
    uint8_t can_data[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

    struct task task_read_sensors = { &read_sensors };

    clock_init();


    gpio_init();

    uart_init();

    int cnt = 10000;
    while (cnt--);
    can_init(&can_h, CAN);

    i2c_master_init(&i2c_h, I2C2);
    hdc_1080_init(&hdc1080_h, &i2c_h);

    hdc_1080_read_temp(&hdc1080_h);
    hdc_1080_read_humidity(&hdc1080_h);

    can_send(&can_h, 0x5, can_data);

    bool led = 1;

    display_set_temperature(&dspl_h, hdc1080_h.temp);

    scheduler_init();
    scheduler_add_task(&task_read_sensors);

    while (1) {
        uart_send();

        struct task *current_task = scheduler_get_next_task();
        current_task->run();

        can_send(&can_h, 0x5, can_data);

        if (led) {
            GPIOB->ODR |= (GPIO_ODR_14 | GPIO_ODR_15);
        } else {
            GPIOB->ODR &= ~(GPIO_ODR_14 | GPIO_ODR_15);
        }

        led = !led;

        //display_set_integer(&dspl_h, systick_ms / 1000);
        display_update(&dspl_h);

        // idle until next 1ms iteration
        //systick_idle();
    }
}
