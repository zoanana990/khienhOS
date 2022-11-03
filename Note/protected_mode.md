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
> The Non-Maskable Interrupt (NMI) is a hardware-driven interrupt much like the PIC interrupts, but the NMI goes either directly to the CPU, or via another controller (e.g., the ISP)---in which case it can be masked.


### A20 Line
> Definition in wiki:


How to enable A20 line:
1. keyboard controller
2. Fast A20 gate
3. Access of 0xee

### Global Descriptor Table
Definition in wiki:
The Global Descriptor Table (GDT) is a binary data structure specific to the IA-32 and x86-64 architectures. It contains entries telling the CPU about memory segments. 



Reference:
[分段架構](https://www.csie.ntu.edu.tw/~wcchen/asm98/asm/proj/b85506061/chap2/segment.html)