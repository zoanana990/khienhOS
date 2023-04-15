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

/**
 * Here is the filesystem function pointer
 * */
typedef void *(*fs_open_func_t)(disk_t *disk, path_t *path, file_mode_t mode);
typedef s32   (*fs_resolve_func_t)(disk_t *disk);

/**
 * @struct: filesystem
 * @open: a function pointer point to the filesystem open function
 *        the open function is used to open a file, a folder etc.
 * @resolve: a function pointer point to the filesystem resolve function
 *           The resolve function is a algorithm on a file system that return a path from another path.
 *           It is equivalent to the execution of change directory (cd) on every argument but it apply also to file.
 * @name: a filesystem name
 *
 * @description:
 *       This structure is a virtual filesystem structure, we need to point to a real filesystem
 *       e.g. FAT16, EXT2, EXT3 etc.
 * */
typedef struct filesystem
{
    fs_open_func_t    open;
    fs_resolve_func_t resolve;

    /* filesystem name */
    s8 name[20];
} fs_t;

/**
 * @struct: file_descriptor
 * @index: the file descriptor index
 * @filesystem: the file descriptor belongs to which filesystem
 * @private: the private data in the file descriptor
 * @disk: the disk that the file descriptor should be used on
 * */
typedef struct file_descriptor
{
    s32   index;
    fs_t *filesystem;
    void *private;
    disk_t *disk;
} fd_t;

/**
 * @func: fs_init
 * @description:
 *      the filesystem initialization.
 * */
void fs_init();
s32 fopen(const s8 *filename, const s8 *mode);
fs_t *fs_resolve(disk_t *disk);

#endif //KHIENHOS_FILE_H