#include <stdint.h>
#include <stdbool.h>

#include "CMSIS/Device/ST/STM32F0xx/Include/stm32f072xb.h"

void clock_init()
{
    // just use the default internal RC oscillator for now

    // enable GPIO peripheral clock for port B
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
}

void gpio_init()
{
    // set B14 and B15 to output
    GPIOB->MODER |=  GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0;

}


int main()
{
    clock_init();

    gpio_init();

    int i;
    bool led = 1;
    while (1) {
        i++;

        if (led) {
            GPIOB->ODR |= (GPIO_ODR_14 | GPIO_ODR_15);
        } else {
            GPIOB->ODR &= ~(GPIO_ODR_14 | GPIO_ODR_15);
        }

        led = !led;

        // sleep some time
        for (int a = 1000000; a > 0; a--);
    }
}
