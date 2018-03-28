/**
 * @file hdc1080.h
 *
 * Implements access to the HDC1080 I2C temperature and humidity sensor
 */
#ifndef CANNODE_HDC1080_H
#define CANNODE_HDC1080_H

#include "i2c.h"

#define HDC1080_I2C_ADDR 0x40
#define HDC1080_REG_TEMP 0x00
#define HDC1080_REG_HUM 0x01
#define HDC1080_REG_CFG 0x02

struct hdc1080 {
    struct i2c *i2c;
    float temp; // the current temperatur
    int humidity; //the current humidity
};

void hdc_1080_init(struct hdc1080 *h, struct i2c *i2c);

void hdc_1080_read_temp(struct hdc1080 *h);

void hdc_1080_read_humidity(struct hdc1080 *h);

#endif // CANNODE_HDC1080_H
