#ifndef KHIENHOS_FAT16_H
#define KHIENHOS_FAT16_H

#include <fs/file.h>
#include <common/string.h>

#define KHIENHOS_FAT16_SIGNATURE        0x29
#define KHIENHOS_FAT16_FAT_ENTRY_SIZE   0x02
#define KHIENHOS_FAT16_BAD_SECTOR       0xFF7
#define KHIENHOS_FAT16_UNUSED           0x00

typedef u32 fat_item_t;
#define FAT_ITEM_TYPE_DIRECTORY         0
#define FAT_ITEM_TYPE_FILE              1

/* Fat directory entry attributes bitmask */
#define FAT_FILE_READ_ONLY              0x01
#define FAT_FILE_HIDDEN                 0x02
#define FAT_FILE_SYSTEM                 0x04
#define FAT_FILE_VOLUME_LABEL           0x08
#define FAT_FILE_SUBDIRECTORY           0x10
#define FAT_FILE_ARCHIVED               0x20
#define FAT_FILE_DEVICE                 0x40
#define FAT_FILE_RESERVED               0x80

/* These two structure are the bootloader instruction there
 * */
typedef __attribute__((packed)) struct fat_header_extended
{
    u8  drive_number;
    u8  win_nt_bit;
    u8  signature;
    u32 volume_id;
    u8  volume_id_string[11];
    u8  system_id_string[8];
} fat_header_extended_t;

typedef __attribute__((packed)) struct fat_header
{
    u8  short_jmp_ins[3];
    u8  oem_identifier[8];
    u16 bytes_per_sector;
    u8  sectors_per_cluster;
    u16 reserved_sectors;
    u8  fat_copies;
    u16 root_dir_entries;
    u16 number_of_sectors;
    u8  media_type;
    u16 sectors_per_fat;
    u16 sectors_per_track;
    u16 number_of_heads;
    u32 hidden_setors;
    u32 sectors_big;
} fat_header_t;



void *fat16_open(disk_t *disk, path_t *path, file_mode_t mode);
s32   fat16_resolve(disk_t *disk);
fs_t *fat16_init();

#endif //KHIENHOS_FAT16_H