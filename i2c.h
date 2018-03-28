/**
 * @file i2c.h
 *
 * I2C Master module
 */
#ifndef CANNODE_I2C_H
#define CANNODE_I2C_H

#include "stm32f072xb.h"

struct i2c {
    I2C_TypeDef *I2C;
};

void i2c_master_init(struct i2c *h, I2C_TypeDef *dev);

void i2c_master_write(struct i2c *h, uint16_t saddr, uint8_t *data,
    uint8_t length);

void i2c_master_read(struct i2c *h, uint16_t saddr, uint8_t *data,
    uint8_t length);



#endif // CANNODE_I2C_H
