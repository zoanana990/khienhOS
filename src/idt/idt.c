#include <idt/idt.h>
#include <khienh/config.h>
#include <khienh/kernel.h>
#include <memory/memory.h>

struct idt_desc idt_descriptors[KHIENHOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

extern void idt_load(struct idtr_desc* ptr);

void idt_zero(struct idtr_desc *ptr)
{
    print("divide by zero error \n");
}

void idt_set(i32 interrupt_no, void* address)
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

    idt_set(0, idt_zero);

    /* load the interrupt descriptor table */
    idt_load(&idtr_descriptor);
}