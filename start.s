.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb

.section .isr_vector, "a"
.word _stack_end // initial SP at end of RAM
.word reset_handler //reset
.word failure_handler // NMI
.word failure_handler // HardFault
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word failure_handler // SVCall
.word 0
.word 0
.word failure_handler // PendSV
.word systick_handler // SysTick
.word failure_handler // WWDG
.word failure_handler // PVD_VDDIO2
.word failure_handler // RTC
.word failure_handler // FLASH
.word failure_handler // RCC_CRS
.word failure_handler // EXTI0_1
.word failure_handler // EXTI2_3
.word failure_handler // EXTI4_15
.word failure_handler // TSC
.word failure_handler // DMA_CH1
.word failure_handler // DMA_CH2_3 DMA2_CH1_2
.word failure_handler // DMA_CH4_5_6_7 DMA2_CH3_4_5
.word failure_handler // ADC_COMP
.word failure_handler // TIM1_BRK_UP_TRG_COM
.word failure_handler // TIM1_CC
.word failure_handler // TIM2
.word failure_handler // TIM3
.word failure_handler // TIM6_DAC
.word failure_handler // TIM7
.word failure_handler // TIM14
.word failure_handler // TIM15
.word failure_handler // TIM16
.word failure_handler // TIM17
.word failure_handler // I2C1
.word failure_handler // I2C2
.word failure_handler // SPI1
.word failure_handler // SPI2
.word usart1_isr      // USART1
.word failure_handler // USART2
.word failure_handler // USART3_4_5_6_7_8
.word can_isr         // CEC_CAN
.word failure_handler // USB



.section .text

.weak reset_handler
.type reset_handler, %function
reset_handler:
    ldr r0, =_stack_end
    mov sp, r0

    ldr r1, =__bss_start__
    mov r2, r1
    ldr r3, =__bss_end__
    movs r4, #0
loop_fillzero_bss:
    cmp r2, r3
    beq fillzero_bss_end
    str r4, [r2]
    adds r2, #4
    b loop_fillzero_bss
fillzero_bss_end:

    bl main
    b failure_handler

.weak systick_handler
.type systick_handler, %function
systick_handler:
b .

.weak failure_handler
.type failure_handler, %function
failure_handler:
b .
