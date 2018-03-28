#include <stdint.h>
#include <stdbool.h>
#include <stm32f072xb.h>
#include "display.h"
#include "hdc1080.h"

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
        uint32_t tmp = 0;

        tmp = I2C2->CR2;
        tmp &= ~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP));
        tmp |= (uint32_t)(((uint32_t)saddr & I2C_CR2_SADD) | (((uint32_t)length << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | I2C_CR2_AUTOEND | I2C_CR2_START );
        I2C2->CR2 = tmp;

//        while (length-- > 0) {
            while (!(I2C2->ISR & I2C_ISR_TXIS));

            I2C2->TXDR = *data++; /* Byte to send */
//        }
    }


}

void i2c_master_read(uint16_t saddr, uint8_t *data, uint8_t length)
{
    uint32_t tmp = I2C2->CR2;
    tmp &= ~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP));
    tmp |= (uint32_t)(((uint32_t)saddr & I2C_CR2_SADD) | (((uint32_t)length << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | I2C_CR2_AUTOEND | I2C_CR2_START | I2C_CR2_RD_WRN);
    I2C2->CR2 = tmp;

    int cnt = 0;
    while (cnt < length) {
        while (!(I2C2->ISR & I2C_ISR_RXNE));
        data[cnt++] = I2C2->RXDR;
    }
}


int main()
{
    clock_init();

    gpio_init();

    i2c2_init();

    uint8_t str[3] = { HDC1080_REG_TEMP };

    i2c_master_send(HDC1080_I2C_ADDR << 1, str, 1);
    uint32_t cnt = 10000;
    while (cnt-- > 0);
    i2c_master_read(HDC1080_I2C_ADDR << 1, str, 2);

    int32_t tmp = (str[0] << 8 |  str[1]) * 165;
    float temp = (float)tmp / (1<<16) - 40;

    struct display dspl = {0};

    int i;
    bool led = 1;

    display_set(&dspl, 0, 1, 2, 3, 1, 0);


    while (1) {
        i++;

        str[0] = HDC1080_REG_TEMP;
        i2c_master_send(HDC1080_I2C_ADDR << 1, str, 1);

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
