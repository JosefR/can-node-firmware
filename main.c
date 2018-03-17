#include <stdint.h>
#include <stdbool.h>

#include "CMSIS/Device/ST/STM32F0xx/Include/stm32f072xb.h"

void clock_init()
{
    // just use the default internal RC oscillator for now

    // enable GPIO peripheral clock for port A
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
}

void gpio_init()
{
//    *(GPIO_B_MODER) |= (0x50000000); // GPIO B14 and B15 are outputs

    // set PA5 to output
    GPIOA->MODER |=  GPIO_MODER_MODER5_0;

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
            GPIOA->ODR |= (GPIO_ODR_5);
        } else {
            GPIOA->ODR &= ~(GPIO_ODR_5);
        }

        led = !led;

        // sleep some time
        for (int a = 1000000; a > 0; a--);
    }
}
