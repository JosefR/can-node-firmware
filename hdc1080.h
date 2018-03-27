/**
 * @file hdc1080.h
 *
 * Implements access to the HDC1080 I2C temperature and humidity sensor
 */
#ifndef CANNODE_HDC1080_H
#define CANNODE_HDC1080_H

struct hdc1080 {
    void *abc;
};

void hdc_1080_init(struct hdc1080 *h);

#endif // CANNODE_HDC1080_H
