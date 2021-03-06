/**
 * @file display.c
 */
#include <stm32f072xb.h>
#include "display.h"

const uint8_t SEVENSEGMENT_DIGITS[] = {
    0b00111111, /* 0 */
    0b00000110, /* 1 */
    0b01011011, /* 2 */
    0b01001111, /* 3 */
    0b01100110, /* 4 */
    0b01101101, /* 5 */
    0b01111101, /* 6 */
    0b00100111, /* 7 */
    0b01111111, /* 8 */
    0b01101111, /* 9 */
    0b01110111, /* A */
    0b01111100, /* B */
    0b00111001, /* C */
    0b01011110, /* D */
    0b01111001, /* E */
    0b01110001, /* F */
    0b00000000, /* all off */
    0b11111111, /* all on */
    0b01000000, /* - */
    0b01110110  /* H */
};

const uint8_t SEVENSEGMENT_DOTS[] = {
    0b00000000, /*  */
    0b00000001, /*  */
    0b00000010, /*  */
    0b00000011, /*  */
    0b00000100 /*  */
};


const uint16_t DIGIT_SELECT[] = {
    0b0001011100000000,
    0b0001101100000000,
    0b0001110100000000,
    0b0001111000000000,
    0b0000111100000000
};

void display_set(struct display *d, uint8_t digit1, uint8_t digit2,
    uint8_t digit3, uint8_t digit4, uint8_t dot, uint8_t dx)
{
    d->digit[0] = SEVENSEGMENT_DIGITS[digit1];
    d->digit[1] = SEVENSEGMENT_DIGITS[digit2];
    d->digit[2] = SEVENSEGMENT_DIGITS[digit3];
    d->digit[3] = SEVENSEGMENT_DIGITS[digit4];
    d->digit[4] = SEVENSEGMENT_DOTS[dx];

    if (dot != DISPLAY_NODOT) {
        d->digit[dot] |= 0b10000000;
    }
}

void display_update(struct display *d)
{
    GPIOC->ODR = ((GPIOC->ODR) & 0xe000)
        | (DIGIT_SELECT[d->current] | (d->digit[d->current] & 0xff));
    if (++(d->current) == 5)
        d->current = 0;
}

void display_set_temperature(struct display *d, float temp)
{
    uint8_t neg = temp < 0;
    display_set(d, 0xc, (int)(temp * 10.0f) % 10, (int)temp % 10,
        neg ? (
            (temp < 10) ? DISPLAY_SEVENSEGMENT_MINUS
                : ((int)(temp / 10.0f) % 10)
        ) : ((temp < 10) ? DISPLAY_SEVENSEGMENT_OFF : ((int)(temp / 10.0f) % 10)
        ), 2, DISPLAY_DOTS_UP );
}

void display_set_integer(struct display *d, int val)
{
    char neg = val < 0;
    char invalid = 0;

    if (neg) {
        val = -val;
        if (val > 999)
            invalid = 1;
    } else {
        if (val > 9999)
            invalid = 1;
    }

    char num_digit = val < 10 ? 1 : (val < 100 ? 2 : (val < 1000 ? 3 : 4));

    if (!invalid) {
        display_set(d,
            val % 10,
            num_digit > 1 ? (val % 100 / 10) : DISPLAY_SEVENSEGMENT_OFF,
            num_digit > 2 ? (val % 1000 / 100) : DISPLAY_SEVENSEGMENT_OFF,
            num_digit > 3 ? (val % 10000 / 1000) : (neg ?
                DISPLAY_SEVENSEGMENT_MINUS : DISPLAY_SEVENSEGMENT_OFF),
            DISPLAY_NODOT, DISPLAY_DOTS_OFF);
    }
}
