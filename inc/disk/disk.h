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

/**
 * @structure: disk
 * @type:
 * @sector_size: the sector size on the disk, here we use 512 bytes
 * @filesystem: The filesystem we use on the disk
 * @id: The id of the disk
 * @fs_private: The private data of our filesystem
 * */
typedef struct disk
{
    kdisk_t type;
    s32     sector_size;
    fs_t   *filesystem;
    s32     id;
    void   *fs_private;
} disk_t;

/**
 * @func: disk_search_and_init
 * @description:
 *  this is used to initialize the global variable `disk`
 *  1. set the disk type
 *  2. set the sector size of the disk
 *  3. set the filesystem on the disk
 * */
void disk_search_and_init();

/**
 * @func: disk_get
 * @id: disk id
 * @description:
 *  we can get the disk from the given id, however,
 *  currently we can only get the only disk, disk 0
 * */
disk_t *disk_get(s32 id);


s32 disk_read_block(disk_t *local_disk, u32 lba, s32 total, void *buf);

#endif //KHIENHOS_DISK_H