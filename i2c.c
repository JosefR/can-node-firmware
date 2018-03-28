/**
 * @file i2c.c
 */
#include "i2c.h"


void i2c_master_init(struct i2c *h, I2C_TypeDef *dev)
{
    h->I2C = dev;

    h->I2C->CR1 &= ~I2C_CR1_PE; // disable and reset
    while ((h->I2C->CR1 & I2C_CR1_PE) != 0); // wait until in reset

    h->I2C->TIMINGR = (uint32_t)0x2000090e;
    //h->I2C->CR2 = I2C_CR2_AUTOEND | (1<<16) | I2C_CR2_RD_WRN
    //    | (0x00 << 1);
    //h->I2C->CR1 = I2C_CR1_RXIE;

    h->I2C->CR1 |= I2C_CR1_PE; // enable
}

void i2c_master_write(struct i2c *h, uint16_t saddr, uint8_t *data, uint8_t length)
{
    /* Check Tx empty */
    if ((h->I2C->ISR & I2C_ISR_TXE) == I2C_ISR_TXE)
    {
        uint32_t tmp = 0;

        tmp = h->I2C->CR2;
        tmp &= ~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP));
        tmp |= (uint32_t)(((uint32_t)saddr & I2C_CR2_SADD) | (((uint32_t)length << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | I2C_CR2_AUTOEND | I2C_CR2_START );
        h->I2C->CR2 = tmp;

        //        while (length-- > 0) {
        while (!(h->I2C->ISR & I2C_ISR_TXIS));

        h->I2C->TXDR = *data++; /* Byte to send */
        //        }
    }
}

void i2c_master_read(struct i2c *h, uint16_t saddr, uint8_t *data, uint8_t length)
{
    uint32_t tmp = h->I2C->CR2;
    tmp &= ~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP));
    tmp |= (uint32_t)(((uint32_t)saddr & I2C_CR2_SADD) | (((uint32_t)length << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | I2C_CR2_AUTOEND | I2C_CR2_START | I2C_CR2_RD_WRN);
    h->I2C->CR2 = tmp;

    int cnt = 0;
    while (cnt < length) {

        // wait until receive register is set
        while ((h->I2C->ISR & I2C_ISR_RXNE) != I2C_ISR_RXNE);

        data[cnt++] = (uint8_t)(h->I2C->RXDR);
    }
}

