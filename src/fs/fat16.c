#include <fs/fat16.h>
#include <memory/memory.h>
#include <memory/kheap.h>

/******************************
 * Static Variable
 ******************************/
fs_t fat16_fs =
{
    .open = fat16_open,
    .resolve = fat16_resolve,
};

/******************************
 * Local Function
 ******************************/
static void fat16_init_private(disk_t *disk, fat_private_t *fat_private)
{
    return;
}

/******************************
 * Exported function
 ******************************/
fs_t *fat16_init()
{
    strcpy(fat16_fs.name, "FAT16");
    return NULL;
}

void *fat16_open(disk_t *disk, path_t *path, file_mode_t mode)
{
    return NULL;
}

s32 fat16_get_root_directory(disk_t *disk, fat_private_t *fat_private, fat_directory_t *directory)
{
    kerr_no_t  ret = kerr_OK;


    return ret;
}

s32 fat16_resolve(disk_t *disk)
{
    kerr_no_t ret = kerr_OK;

    /* init the private data */
    fat_private_t *fat_private = kzalloc(sizeof(fat_private_t));
    fat16_init_private(disk, fat_private);

    disk->fs_private = fat_private;
    disk->filesystem = &fat16_fs;

    ds_t* stream = disk_streamer_new(disk->id);
    if(!stream)
    {
        ret = -kerr_NOMEM;
        goto out;
    }

    if (disk_streamer_read(stream, &fat_private->header, sizeof(fat_private->header)) != kerr_OK)
    {
        ret = -kerr_IO;
        goto out;
    }

    if (fat_private->header.shared.extended_header.signature != 0x29)
    {
        ret = -kerr_FSNOTUS;
        goto out;
    }

    if (fat16_get_root_directory(disk, fat_private, &fat_private->root_directory) != kerr_OK)
    {
        ret = -kerr_IO;
        goto out;
    }

    out:
    return ret;
}