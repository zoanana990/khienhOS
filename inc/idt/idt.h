#ifndef IDT_H
#define IDT_H

#include <khienh/type.h>

struct idt_desc{
    u16 offset_1; /* offset bit 0 - 15 */
    u16 selector; /* a code segment selector in GDT or LDT */
    u8  zero;     /* unused, set to 0 */
    u8  type_attr;/* type and attributes, see below */
    u16 offset_2; /* offset bits 16 - 31*/
}__attribute__((packed));

struct idtr_desc
{
    u16 limit;    /* size of descriptor table - 1*/
    u32 base;     /* base address of the start of the interrupt descriptor table */
}__attribute__((packed));

#endif