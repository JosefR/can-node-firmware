/**
 * @file display.c
 *
 * Module to control the seven segment display.
 */

#define DISPLAY_DOTS_OFF 0
#define DISPLAY_DOTS_MIDDLE 3
#define DISPLAY_DOTS_UP 4

#define DISPLAY_NODOT 4 // no comma/dot

struct display {
    uint8_t digit[5];
    uint8_t current; // current digit
};

void display_set(struct display *d, uint8_t digit1, uint8_t digit2,
    uint8_t digit3, uint8_t digit4, uint8_t dot, uint8_t dx);

void display_update(struct display *d);
