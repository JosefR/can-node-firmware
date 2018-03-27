#include <stdint.h>
#include <stdbool.h>
#include <stm32f072xb.h>
#include "display.h"

void clock_init()
{
    // just use the default internal RC oscillator for now

    // enable GPIO peripheral clock for port B and C
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;

    I2C2->TIMINGR = (uint32_t)0x2000090E;
}

void gpio_init()
{
    // set B14 and B15 to output
    GPIOB->MODER |=  GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0;

    // set C0 - C12 to output
    GPIOC->MODER |=  GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 |
        GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 |
        GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 |
        GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER10_0 |
        GPIO_MODER_MODER11_0 | GPIO_MODER_MODER12_0;
}

void i2c_init()
{
    I2C2->TIMINGR = (uint32_t)0x2000090E;
    I2C2->CR1 = I2C_CR1_PE | I2C_CR1_RXIE;
    I2C2->CR2 = I2C_CR2_AUTOEND | (1<<16) | I2C_CR2_RD_WRN
        | (I2C1_OWN_ADDRESS << 1);
}

void i2c_master_send(uint16_t saddr, uint8_t *data, uint8_t length)
{
    /* Check Tx empty */
    if ((I2C2->ISR & I2C_ISR_TXE) == I2C_ISR_TXE)
    {
        I2C2->TXDR = I2C_BYTE_TO_SEND; /* Byte to send */
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
