.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb


.section .isr_vector, "x"
.word _stack_start // initial SP at end of RAM
.word reset_handler //reset
.word failure_handler // NMI
.word failure_handler // HardFault
.word failure_handler // SVCall
.word failure_handler // PendSV
.word failure_handler // SysTick
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
.word failure_handler // USART1
.word failure_handler // USART2
.word failure_handler // USART3_4_5_6_7_8
.word failure_handler // CEC_CAN
.word failure_handler // USB


.global reset_handler
reset_handler:
/*ldr r0, =_stack_start
mov sp, r0*/
bl main
b failure_handler


failure_handler:
b .
