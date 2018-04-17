/**
 * @file systick.h
 *
 * Systick handling
 */
#ifndef CANNODE_SYSTICK_H
#define CANNODE_SYSTICK_H

extern volatile uint64_t systick_ms;

void systick_init(uint32_t ticks);

void systick_idle();

#endif // CANNODE_SYSTICK_H
