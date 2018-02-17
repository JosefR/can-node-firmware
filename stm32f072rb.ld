ENTRY(reset_handler)

MEMORY
{
    flash (rx)  : ORIGIN = 0x08000000, LENGTH = 128K
    ram (xrw)   : ORIGIN = 0x20000000, LENGTH = 16K
}

_estack = 0x20004000;

SECTIONS
{
    .text :
    {
    } >flash

    .data
    {
    } >ram
}
