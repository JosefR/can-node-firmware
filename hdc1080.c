/**
 * @file hdc1080.h
 */
#include "hdc1080.h"


void hdc_1080_init(struct hdc1080 *h, struct i2c *i2c)
{
    h->i2c = i2c;
}

void hdc_1080_read_temp(struct hdc1080 *h)
{
    uint8_t data[2] = { HDC1080_REG_TEMP };

    i2c_master_write(h->i2c, HDC1080_I2C_ADDR << 1, data, 1);
    uint32_t cnt = 100000;
    while (cnt-- > 0);
    i2c_master_read(h->i2c, HDC1080_I2C_ADDR << 1, data, 2);

    int32_t tmp = (data[0] << 8 |  data[1]) * 165;
    h->temp = (float)tmp / (1<<16) - 40;
}

void hdc_1080_read_humidity(struct hdc1080 *h)
{
    uint8_t data[2] = { HDC1080_REG_HUM };

    i2c_master_write(h->i2c, HDC1080_I2C_ADDR << 1, data, 1);
    uint32_t cnt = 100000;
    while (cnt-- > 0);
    i2c_master_read(h->i2c, HDC1080_I2C_ADDR << 1, data, 2);

    int tmp = (data[0] << 8 |  data[1]) * 100;
    h->humidity = tmp / (1<<16);
}
