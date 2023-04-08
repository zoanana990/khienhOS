#ifndef KHIENHOS_FAT16_H
#define KHIENHOS_FAT16_H

#include <fs/file.h>
#include <common/string.h>
#include <disk/stream.h>

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
typedef struct fat_header_extended
{
    u8  drive_number;
    u8  win_nt_bit;
    u8  signature;
    u32 volume_id;
    u8  volume_id_string[11];
    u8  system_id_string[8];
} __attribute__((packed)) fat_header_extended_t;

typedef struct fat_header
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
} __attribute__((packed)) fat_header_t;

typedef struct fat_h{
    fat_header_t primary_header;
    union fat_f_u{
        fat_header_extended_t extended_header;
    } shared;
} fat_h_t;

typedef struct fat_directory_item
{
    u8  filename[8];
    u8  ext[3];
    u8  attribute;
    u8  reserved;
    u8  creation_time_tenths_of_a_sec;
    u16 creation_time;
    u16 creation_date;
    u16 last_access;
    u16 high_16_bits_first_cluster;
    u16 last_mod_time;
    u16 last_mod_date;
    u16 low_16_bits_first_cluster;
    u32 filesize;
} __attribute__((packed)) fat_directory_item_t;

typedef struct fat_directory
{
    fat_directory_item_t *item;
    s32 total;
    s32 sector_pos;
    s32 ending_sector_pos;
} fat_directory_t;

typedef struct fat_item
{
    union
    {
        struct fat_directory_item* item;
        struct fat_directory* directory;
    };

    fat_item_t type;
} fat_item_structure_t;

typedef struct fat_item_descriptor
{
    fat_item_structure_t * item;
    u32 pos;
} fat_item_descriptor_t;

typedef struct fat_private
{
    fat_h_t header;
    fat_directory_t root_directory;

    /* Used to stream data clusters */
    ds_t* cluster_read_stream;

    /* Used to stream the file allocation table */
    ds_t* fat_read_stream;

    /* Used in situations where we stream the directory */
    ds_t* directory_stream;
} fat_private_t;

void *fat16_open(disk_t *disk, path_t *path, file_mode_t mode);
s32   fat16_resolve(disk_t *disk);
fs_t *fat16_init();

#endif //KHIENHOS_FAT16_H