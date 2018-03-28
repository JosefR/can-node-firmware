/**
 * @file display.h
 *
 * Module to control the seven segment display.
 */
#ifndef CANNODE_DISPLAY_H
#define CANNODE_DISPLAY_H

#define DISPLAY_DOTS_OFF 0
#define DISPLAY_DOTS_MIDDLE 3
#define DISPLAY_DOTS_UP 4

#define DISPLAY_SEVENSEGMENT_OFF 16
#define DISPLAY_SEVENSEGMENT_ALL 17
#define DISPLAY_SEVENSEGMENT_MINUS 18
#define DISPLAY_SEVENSEGMENT_H 19

#define DISPLAY_NODOT 4 // no comma/dot

struct display {
    uint8_t digit[5];
    uint8_t current; // current digit
};

void display_set(struct display *d, uint8_t digit1, uint8_t digit2,
    uint8_t digit3, uint8_t digit4, uint8_t dot, uint8_t dx);

void display_set_temperature(struct display *d, float temp);

void display_update(struct display *d);

#endif // CANNODE_DISPLAY_H
