#ifndef KHIENHOS_FAT16_H
#define KHIENHOS_FAT16_H

#include <fs/file.h>

void *fat16_open(disk_t *disk, path_t *path, file_mode_t mode);
s32   fat16_resolve(disk_t *disk);

fs_t *fat16_init();


#endif //KHIENHOS_FAT16_H
