.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb


.long _estack ; initial SP at end of RAM
.long _start

_start:
loop:
    ADD R0, R0, #1
    b loop


.global _start
