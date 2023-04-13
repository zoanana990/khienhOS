#ifndef _CONFIG_H_
#define _CONFIG_H_

#define KHIENHOS_KERNEL_VERSION     "v0.1"

/* code segment and data segment, same as boot.asm */
#define KERNEL_CODE_SELECTOR        0x08
#define KERNEL_DATA_SELECTOR        0x10

/* OS config */
#define KHIENHOS_TOTAL_INTERRUPTS   512

/* Max memory allocation */
#define KHIENHOS_HEAP_MAX_SIZE      0x10000000
#define KHIENHOS_HEAP_BLOCK_SIZE    0x1000
#define KHIENHOS_HEAP_START_ADDR    0x1000000
#define KHIENHOS_HEAP_TABLE_ADDR    0x7E00

/* disk config */
#define KHIENHOS_DISK_SECTOR_SIZE   512

/* file system */
#define KHIENHOS_MAX_FILESYSTEMS      12
#define KHIENHOS_MAX_FILE_DESCRIPTORS 512
#define KHIENHOS_MAX_PATH_SIZE        512

#endif