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

s32 fat16_resolve(disk_t *disk)
{
    kerr_no_t ret = kerr_OK;

    /* init the private data */
    fat_private_t *fat_private = kzalloc(sizeof(fat_private_t));
    fat16_init_private(disk, fat_private);

    return ret;
}