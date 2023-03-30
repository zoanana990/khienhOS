#ifndef CONFIG_H
#define CONFIG_H

/* code segment and data segment, same as boot.asm */
#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10

/* OS config */
#define KHIENHOS_TOTAL_INTERRUPTS 512

#endif