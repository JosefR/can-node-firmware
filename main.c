#include <stdint.h>
#include <stdbool.h>
#include <stm32f072xb.h>
#include "display.h"

void clock_init()
{
    // just use the default internal RC oscillator for now

    // enable GPIO peripheral clock for port B and C
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;

    // enable I2C2 Clock
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

}

void gpio_init()
{
    // set B14 and B15 to output, B9 and B10 to alternate function
    GPIOB->MODER |=  GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0
        | GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;

    // set alternate functions: GPIOB Pin 10: I2C2_SDL, Pin 11: I2C2_SDA
    GPIOB->AFR[1] |= 0x1 << GPIO_AFRH_AFRH2_Pos | 0x01 << GPIO_AFRH_AFRH3_Pos;


    // set C0 - C12 to output
    GPIOC->MODER |=  GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 |
        GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 |
        GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 |
        GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER10_0 |
        GPIO_MODER_MODER11_0 | GPIO_MODER_MODER12_0;
}

void i2c2_init()
{
    I2C2->CR1 &= ~I2C_CR1_PE; // disable and reset

    I2C2->TIMINGR = (uint32_t)0x2000090e;
    I2C2->CR2 = I2C_CR2_AUTOEND | (1<<16) | I2C_CR2_RD_WRN
        | (0x00 << 1);
    I2C2->CR1 = I2C_CR1_RXIE;


    I2C2->CR1 |= I2C_CR1_PE; // enable

}

void i2c_master_send(uint16_t saddr, uint8_t *data, uint8_t length)
{
    /* Check Tx empty */
    if ((I2C2->ISR & I2C_ISR_TXE) == I2C_ISR_TXE)
    {
        I2C2->TXDR = data[0]; /* Byte to send */
        I2C2->CR2 |= I2C_CR2_START; /* Go */
    }
}


int main()
{
    clock_init();

    gpio_init();

    struct display dspl = {0};

    int i;
    bool led = 1;

    display_set(&dspl, 0, 1, 2, 3, 1, 0);


    while (1) {
        i++;

        if (led) {
            GPIOB->ODR |= (GPIO_ODR_14 | GPIO_ODR_15);
        } else {
            GPIOB->ODR &= ~(GPIO_ODR_14 | GPIO_ODR_15);
        }

        led = !led;

        display_update(&dspl);

        // sleep some time
        //for (int a = 100; a > 0; a--);
    }
}
