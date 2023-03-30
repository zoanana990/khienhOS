#ifndef _CONFIG_H_
#define _CONFIG_H_

/* code segment and data segment, same as boot.asm */
#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10

/* OS config */
#define KHIENHOS_TOTAL_INTERRUPTS 512

#endif