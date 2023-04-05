#ifndef KHIENHOS_DISK_H
#define KHIENHOS_DISK_H

#include <khienh/type.h>
#include <khienh/config.h>
#include <memory/memory.h>
#include <io/io.h>
#include <fs/file.h>

typedef u32 kdisk_t;

#define KHIENHOS_DISK_TYPE_REAL 0

typedef struct filesystem fs_t;
typedef struct disk
{
    kdisk_t type;
    s32     sector_size;
    fs_t   *filesystem;
} disk_t;

disk_t *disk_get(s32 index);
void disk_search_and_init();
s32 disk_read_block(disk_t *local_disk, u32 lba, s32 total, void *buf);

#endif //KHIENHOS_DISK_H