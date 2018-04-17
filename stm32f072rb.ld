ENTRY(reset_handler)

MEMORY
{
    flash (rx)  : ORIGIN = 0x08000000, LENGTH = 128K
    ram (xrw)   : ORIGIN = 0x20000000, LENGTH = 16K
}

_stack_size = 1024;
_stack_end = ORIGIN(ram) + LENGTH(ram); /* end of RAM */
_stack_start = _stack_end - _stack_size;

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >flash

    .text :
    {        
        . = ALIGN(4);
        *(.text)
        . = ALIGN(4);
    } >flash

    .data :
    {
        . = ALIGN(4);
        *(.data)
        . = ALIGN(4);
    } >ram

    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss)
        *(.bss*)
        . = ALIGN(4);
        __bss_end__ = .;
    } >ram

    . = _stack_start;
    ._stack : 
    {
        . = . + _stack_size;
    } >ram
}

