#include <khienh/config.h>
#include <khienh/kernel.h>
#include <memory/memory.h>
#include <idt/idt.h>
#include <io/io.h>

struct idt_desc idt_descriptors[KHIENHOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

extern void idt_load(struct idtr_desc* ptr);
extern void int21h();
extern void no_interrupt();

void int21h_handler()
{
    outb(0x20, 0x20);
}

void no_interrupt_handler()
{
    outb(0x20, 0x20);
}

void idt_zero(struct idtr_desc *ptr)
{
    print("divide by zero error \n");
}

void idt_set(s32 interrupt_no, void* address)
{
    struct idt_desc* desc = &idt_descriptors[interrupt_no];
    desc->offset_1 = (u32) address & 0xffff;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero = 0x00;

    /* set the 40 - 47 bits, p, ring 3 and gate type */
    desc->type_attr = 0xEE;
    desc->offset_2 = (u32) address >> 16;
}

void idt_init()
{
    memset(idt_descriptors, 0, sizeof(idt_descriptors));

    /* The length of the interrupt descriptor table minus one */
    idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
    idtr_descriptor.base = (u32) idt_descriptors;

    /* clear all interrupt handler */
    for(s32 i = 0; i < KHIENHOS_TOTAL_INTERRUPTS; i++)
    {
        idt_set(i, no_interrupt);
    }

    idt_set(0, idt_zero);
    idt_set(0x20, int21h_handler);

    /* load the interrupt descriptor table */
    idt_load(&idtr_descriptor);
}