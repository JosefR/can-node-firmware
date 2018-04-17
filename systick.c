/**
 * @file systick.c
 */

#include <stm32f072xb.h>
#include "systick.h"

volatile uint64_t systick_ms = 0;

static uint64_t last_idle_ms;

void systick_handler()
{
    systick_ms++;
}

void systick_init(uint32_t ticks)
{
    SysTick_Config(ticks);
    NVIC_SetPriority(SysTick_IRQn, 0);
}

void systick_idle()
{
    // idle until next 1ms iteration
    while (1) {
        __disable_irq();
        if (last_idle_ms < systick_ms) {
            last_idle_ms = systick_ms;
            __enable_irq();
            break;
        }
        __enable_irq();

        // reduce irq
        int cnt = 100;
        while (cnt--) {
            __ASM volatile ("nop" : : : );
        }
    }
}
