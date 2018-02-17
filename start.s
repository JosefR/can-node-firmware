.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb


.section .isr_vector
.long _stack_start // initial SP at end of RAM
.long reset_handler

reset_handler:
loop:
    adds r0, r0, #1
    b loop

.global reset_handler
