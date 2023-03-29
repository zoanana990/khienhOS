ENTRY(_start)
OUTPUT_FORMAT(binary)
SECTIONS
{
    /* offset */
    . = 1M;
    .text : ALIGN(4096)
    {
        *(.text)
    }
    .rodata : ALIGN(4096)
    {
        *(.rodata)
    }
    .data : ALIGN(4096)
    {
        *(.data)
    }
    .bss : ALIGN(4096)
    {
        *(COMMON)
        *(.bss)
    }

    /* create an asm section to avoid the alignment issue */
    .asm : ALIGN(4096)
    {
        *(.asm)
    }
}