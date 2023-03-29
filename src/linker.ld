ENTRY(_start)
OUTPUT_FORMAT(binary)
SECTIONS
{
    /* offset */
    . = 1M;
    .text :
    {
        *(.text)
    }
    .rodata :
    {
        *(.rodata)
    }
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(COMMON)
        *(.bss)
    }
}