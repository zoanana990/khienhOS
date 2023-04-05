#ifndef KHIENHOS_FILE_H
#define KHIENHOS_FILE_H

#include <fs/parser.h>
#include <disk/disk.h>
#include <khienh/config.h>

typedef enum {
    SEEK_SET,
    SEEK_CUR,
    SEEK_END
} file_seek_mode_t;

typedef enum {
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_INVALID
} file_mode_t;
typedef struct disk disk_t;
typedef void *(*fs_open_func_t)(disk_t *disk, path_t *path, file_mode_t mode);
typedef s32   (*fs_resolve_func_t)(disk_t *disk);

/*
 * filesystem should return zero from resolve if the provided disk is using its filesystem
 * */
typedef struct filesystem
{
    fs_open_func_t    open;
    fs_resolve_func_t resolve;

    /* filesystem name */
    s8 name[20];
} fs_t;

typedef struct file_descriptor
{
    /* descriptor index */
    s32   index;
    fs_t *filesystem;

    /* private data for internal file descriptor */
    void *private;

    /* the disk that the file descriptor should be used on */
    disk_t *disk;
} fd_t;

void fs_init();
s32 fopen(const s8 *filename, const s8 *mode);
void fs_insert_filesystem(fs_t *filesystem);
fs_t *fs_resolve(disk_t *disk);

#endif //KHIENHOS_FILE_H