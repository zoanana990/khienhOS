#include <fs/file.h>
#include <memory/memory.h>
#include <memory/kheap.h>
#include <khienh/type.h>
#include <disk/disk.h>

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
    // fs_insert_filesystem(fat16_init());
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