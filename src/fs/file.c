#include <fs/file.h>
#include <fs/fat16.h>
#include <memory/memory.h>
#include <memory/kheap.h>
#include <khienh/type.h>
#include <khienh/kernel.h>
#include <disk/disk.h>
#include <common/string.h>

fs_t *filesystems[KHIENHOS_MAX_FILESYSTEMS];
fd_t *file_descriptors[KHIENHOS_MAX_FILE_DESCRIPTORS];

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
    return;
}

void fs_load()
{
    memset(filesystems, 0, sizeof(filesystems));
    fs_static_load();
}

void fs_init()
{
    memset(file_descriptors, 0, sizeof(file_descriptors));
    fs_load();
}

static kerr_no_t file_new_descriptor(fd_t **desc_out)
{
    kerr_no_t ret = kerr_NOMEM;
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

fs_t *fs_resolve(disk_t *disk)
{
    fs_t *fs = 0;

    for(s32 i = 0; i < KHIENHOS_MAX_FILESYSTEMS; i++)
    {
        if (filesystems[i] != 0 && filesystems[i]->resolve(disk) == 0)
        {
            fs = filesystems[i];
            break;
        }
    }

    return fs;
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

s32 fopen(const s8 *filename, const s8 *mode_str)
{
    kerr_no_t ret;

    path_root_t *root_path = path_parser_parse(filename, NULL);

    if(root_path == NULL)
    {
        ret = -kerr_INVARG;
        goto out;
    }

    disk_t *disk = disk_get(root_path->drive_no);

    /* Check the file system exist on disk, we cannot open the file withour the filesystem */
    if(disk == NULL)
    {
        ret = -kerr_IO;
        goto out;
    }

    if(disk->filesystem == NULL)
    {
        ret = -kerr_FSNOTUS;
        goto out;
    }

    file_mode_t mode = file_get_mode_by_string(mode_str);
    if(mode == FILE_MODE_INVALID)
    {
        ret = -kerr_INVARG;
        goto out;
    }

    /* use the fat16_open function */
    void *descriptor_private_data = disk->filesystem->open(disk, root_path->head, mode);
    if(IS_ERR(descriptor_private_data))
    {
        ret = ERROR_I(descriptor_private_data);
        goto out;
    }

    fd_t *desc = NULL;
    ret = file_new_descriptor(&desc);
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
    if(ret < 0) ret = 0;
    return ret;
}