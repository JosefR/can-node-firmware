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

int main()
{
    struct i2c i2c_h;
    struct hdc1080 hdc1080_h;
    struct display dspl_h = {{0}, 0};

    clock_init();

    gpio_init();

    i2c_master_init(&i2c_h, I2C2);
    hdc_1080_init(&hdc1080_h, &i2c_h);

    hdc_1080_read_temp(&hdc1080_h);
    hdc_1080_read_humidity(&hdc1080_h);


    bool led = 1;

    display_set_temperature(&dspl_h, hdc1080_h.temp);
    while (1) {
        if (led) {
            GPIOB->ODR |= (GPIO_ODR_14 | GPIO_ODR_15);
        } else {
            GPIOB->ODR &= ~(GPIO_ODR_14 | GPIO_ODR_15);
        }

        led = !led;

        display_update(&dspl_h);

        // sleep some time
        //for (int a = 100; a > 0; a--);
    }
}
