# Protected Mode
tags: background_knowledge

## What is protected mode

## How to enter the protected mode
Three steps here:
1. Disable interrupts, including [NMI](https://wiki.osdev.org/Non_Maskable_Interrupt) (as suggested by Intel Developers Manual).
2. Enable the [A20 Line](https://wiki.osdev.org/A20_Line).
3. Load the [Global Descriptor Table](https://wiki.osdev.org/Global_Descriptor_Table) with segment descriptors suitable for code, data, and stack.

### Non Maskable Interrupt
> Definition in wiki:
> The Non-Maskable Interrupt (NMI) is a hardware-driven interrupt much like the PIC interrupts, 
> but the NMI goes either directly to the CPU, or via another controller (e.g., the ISP)---in which case it can be masked.


### A20 Line
> Definition in wiki:


How to enable A20 line:
1. keyboard controller
2. Fast A20 gate
3. Access of 0xee

### Global Descriptor Table
Definition in wiki:
The Global Descriptor Table (GDT) is a binary data structure specific to the IA-32 and x86-64 architectures. 
It contains entries telling the CPU about memory segments. 

## Interrupt descriptor table
Source: [osdev Interrupt descriptor table](https://wiki.osdev.org/Interrupt_Descriptor_Table)
- Describe how interrupts are invoked in protected mode
- Can be mapped anywhere in memory
- Different from the interrupt vector table

Interrupt descriptor

| Name     | Bit   | Known as                   | Description                                                             |
|----------|-------|----------------------------|-------------------------------------------------------------------------|
| Offset   | 46-63 | offset 16-31               | The higher part of the offset to execute                                |
| P        | 47    | Present                    | This should be set to zero for unused interrupts                        |
| DPL      | 45-46 | Descriptor Privilege Level | The ring level the processor requires to call this interrupt            |
| S        | 44    | Storage Segment            | Should be set to zero for trap gates                                    |
| Type     | 40-43 | Gate type                  | The type of gate this interrupt is treated as                           |
| O        | 32-49 | Unused 0-7                 | Unused bits in this structure                                           |
| Selector | 16-31 | Selector 0-15              | The selector this interrupt is bounded to i.e. the kernel code selector |
| Offset   | 0-15  | Offset 0-15                | The lower part of the offset to execute                                 |

```c
struct idt_desc{
    u16 offset_1; /* offset bit 0 - 15 */
    u16 selector; /* a code segment selector in GDT or LDT */
    u8 zero;      /* unused, set to 0 */
    u8 type_attr; /* type and attributes, see below */
    u16 offset_2; /* offset bits 16 - 31*/
}__attribute__((packed));
```

Interrupt Gate Types

| Name                        | value       | Description                                                                                                                                              |
|-----------------------------|-------------|----------------------------------------------------------------------------------------------------------------------------------------------------------|
| 80386 32 bit task gate      | 0x05/0b0101 | Tasks gates reference TSS descriptors and cons can assist in multitasking when exceptions occur                                                          |
| 80386 16 bit interrupt gate | 0x06/0b0110 | Interrupts gates are to be used for interrupts that we want ot invoke ourselves in our code                                                              |
| 80386 16 bit trap gate      | 0x07/0b0111 | Trap gates are like interrupt gates however they are used for exceptions. They also disable interrupts on entry and reenable them on an `iret` instruction |
| 80386 32 bit interrupt gate | 0x0E/0b1110 | Interrupts gates are to be used for interrupts that we want ot invoke ourselves in our code                                                              |
| 80386 32 bit trap gate      | 0x0F/0b1111 | Trap gates are like interrupt gates however they are used for exceptions. They also disable interrupts on entry and reenable them on an `iret` instruction |

Interrupt descriptors are stored in an array with index 0 defining interrupt zero `intr. 0` ..., index 1 
```c
struct idt_desc idt_desc[...];
```

IDTR

| Name  | Bit   | Description                                            |
|-------|-------|--------------------------------------------------------|
| Limit | 0-15  | The length of the interrupt descriptor table minus one |
| Base  | 16-47 | The address of the interrupt descriptor                |

```c
struct idtr_desc
{
    u16 limit;
    u32 base;
}__attribute__((packed));
```

Note
- Interrupt descriptor table can be defined where we like in memory
- Interrupt descriptor table are setup differently than the interrupt vector table
- During an interrupt certain property can be pushed to the stack. 
  The rules involved with this are quite complicated so we will discuss them as they come and they do not always apply
- IDT is usually used in protected mode and IVT is used in real mode

## Reference:
- [分段架構](https://www.csie.ntu.edu.tw/~wcchen/asm98/asm/proj/b85506061/chap2/segment.html)
- [Intel Architecture 保護模式架構](https://www.csie.ntu.edu.tw/~wcchen/asm98/asm/proj/b85506061/cover.html)