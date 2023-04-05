#include <fs/fat16.h>

fs_t fat16_fs =
{
    .open = fat16_open,
    .resolve = fat16_resolve,
};

fs_t *fat16_init()
{
    strcpy(fat16_fs.name, "FAT16");
    return NULL;
}

void *fat16_open(disk_t *disk, path_t *path, file_mode_t mode)
{
    return NULL;
}
s32   fat16_resolve(disk_t *disk)
{
    return 0;
}