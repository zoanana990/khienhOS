#include <fs/file.h>
#include <fs/fat16.h>
#include <memory/memory.h>
#include <memory/kheap.h>
#include <khienh/type.h>
#include <khienh/kernel.h>
#include <disk/disk.h>
#include <common/string.h>

/********************************
 * local global variable
 ********************************/

fs_t *filesystems[KHIENHOS_MAX_FILESYSTEMS];
fd_t *file_descriptors[KHIENHOS_MAX_FILE_DESCRIPTORS];

/********************************
 * local function
 ********************************/
static fs_t **fs_get_free_filesystem()
{
    s32 i = 0;
    for(; i < KHIENHOS_MAX_FILESYSTEMS; i++)
        if(filesystems[i] == 0)
            return &filesystems[i];

    return NULL;
}

void fs_insert_filesystem(fs_t *filesystem)
{
    fs_t **fs;
    fs = fs_get_free_filesystem();
    if(!fs)
    {
        print("Insert filesystem error, system pending!!!\n");
        while(1);
    }

    *fs = filesystem;
}

static void fs_static_load()
{
    fs_insert_filesystem(fat16_init());
}



static kerr_no_t file_new_descriptor(fd_t **desc_out)
{
    kerr_no_t ret = -kerr_NOMEM;
    for(s32 i = 0; i < KHIENHOS_MAX_FILE_DESCRIPTORS; i++)
    {
        if(file_descriptors[i] == 0)
        {
            fd_t *desc = kzalloc(sizeof(fd_t));
            desc->index = i + 1;
            file_descriptors[i] = desc;
            *desc_out = desc;
            ret = kerr_OK;
            break;
        }
    }

    return ret;
}

fd_t *file_get_descriptor(s32 fd)
{
    if(fd <= 0 || fd >= KHIENHOS_MAX_FILE_DESCRIPTORS)
        return NULL;

    s32 index = fd - 1;
    return file_descriptors[index];
}

file_mode_t file_get_mode_by_string(const s8 *str)
{
    file_mode_t mode = FILE_MODE_INVALID;

    if(strncmp(str, "r", 1) == 0)
    {
        mode = FILE_MODE_READ;
    }
    else if(strncmp(str, "w", 1) == 0)
    {
        mode = FILE_MODE_WRITE;
    }
    else if(strncmp(str, "a", 1) == 0)
    {
        mode = FILE_MODE_APPEND;
    }

    return mode;
}

/********************************
 * export function
 ********************************/
/**
 * @func: fs_init
 * @description:
 *      the filesystem initialization.
 * */
void fs_init()
{
    memset(file_descriptors, 0, sizeof(file_descriptors));
    memset(filesystems, 0, sizeof(filesystems));
    fs_static_load();
}

/**
 * @func: fs_open
 * @filename: the filename with extension
 * @mode_str: the access mode, e.g., read, write, append etc.
 * @description:
 *      open the file.
 * */
s32 fopen(const s8 *filename, const s8 *mode_str)
{
    s32 ret;

    /*
     * Make a linked list, connect all string to the
     * */
    path_root_t *root_path = path_parser_parse(filename, NULL);

    if(root_path == NULL)
    {
        ret = -kerr_INVARG;
        print("rootpath is NULL ret: %d\n", ret);
        goto out;
    }

    /* if it is successfully */
    disk_t *disk = disk_get(root_path->drive_no);
    /* Check the file system exist on disk, we cannot open the file withour the filesystem */
    if(disk == NULL)
    {
        ret = -kerr_IO;
        print("Disk is NULL ret: %d\n", ret);
        goto out;
    }

    if(disk->filesystem == NULL)
    {
        ret = -kerr_FSNOTUS;
        print("filesystem is NULL ret: %d\n", ret);
        goto out;
    }

    file_mode_t mode = file_get_mode_by_string(mode_str);
    if(mode == FILE_MODE_INVALID)
    {
        ret = -kerr_INVARG;
        print("file_get_mode_by_string ret: %d\n", ret);
        goto out;
    }

    /* use the fat16_open function */
    void *descriptor_private_data = disk->filesystem->open(disk, root_path->head, mode);
    if(IS_ERR(descriptor_private_data))
    {
        ret = ERROR_I(descriptor_private_data);
        print("descriptor private data ret: %d\n", ret);
        goto out;
    }

    fd_t *desc = NULL;
    ret = file_new_descriptor(&desc);
    print("file_new_descriptor ret: %d\n", ret);
    if(ret < 0)
    {
        goto out;
    }

    desc->filesystem = disk->filesystem;
    desc->private = descriptor_private_data;
    desc->disk = disk;
    ret = desc->index;

    out:
    /* fopen should not return negative value */
    return ret < 0 ? 0 : ret;
}

/**
 * @func: fs_resolve
 * @disk: the disk which we put the data on
 * @description:
 *
 * */
fs_t *fs_resolve(disk_t *disk)
{
    fs_t *fs = 0;

    for(s32 i = 0; i < KHIENHOS_MAX_FILESYSTEMS; i++)
    {
        if (filesystems[i] != 0 && filesystems[i]->resolve(disk) == 0)
        {
            fs = filesystems[i];
            print("[%s]: i = %d, filesystem: %s\n", __func__, i, fs->name);
            LOG("filesystem: %s\n", fs->name);
            break;
        }
    }

    return fs;
}