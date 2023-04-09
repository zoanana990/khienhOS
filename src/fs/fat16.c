#include <fs/fat16.h>
#include <memory/memory.h>
#include <memory/kheap.h>
#include <disk/stream.h>

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
static void fat16_init_private(disk_t *disk, fat_private_t *private)
{
    memset(private, 0, sizeof(fat_private_t));
    private->cluster_read_stream = disk_streamer_new(disk->id);
    private->fat_read_stream = disk_streamer_new(disk->id);
    private->directory_stream = disk_streamer_new(disk->id);
}

/******************************
 * Exported function
 ******************************/
fs_t *fat16_init()
{
    strcpy(fat16_fs.name, "FAT16");
    return &fat16_fs;
}

/* we can open fat16 files */
void *fat16_open(disk_t *disk, path_t *path, file_mode_t mode)
{
    if(mode != FILE_MODE_READ)
    {
        return ERROR(-kerr_RDONLY);
    }

    fat_file_desctitor
    return NULL;
}

s32 fat16_sector_to_absolute(disk_t *disk, s32 sector)
{
    return sector * disk->sector_size;
}

s32 fat16_get_total_items_for_directory(disk_t *disk, u32 directory_start_sector)
{
    fat_directory_item_t item;
    fat_directory_item_t empty_item;
    memset(&empty_item, 0, sizeof(fat_directory_item_t));

    fat_private_t *fat_private = disk->fs_private;

    kerr_no_t ret = kerr_OK;
    s32 i = 0;
    s32 directory_start_pos = directory_start_sector * disk->sector_size;
    ds_t *stream = fat_private->directory_stream;
    if(disk_streamer_seek(stream, directory_start_pos) != kerr_OK)
    {
        ret = -kerr_IO;
        goto out;
    }

    while(1)
    {
        if (disk_streamer_read(stream, &item, sizeof(item)) != kerr_OK)
        {
            ret = -kerr_IO;
            goto out;
        }

        if (item.filename[0] == 0x00)
        {
            /* done */
            break;
        }

        /* item is not used*/
        if (item.filename[0] == 0xE5)
        {
            continue;
        }
        i++;
    }
    ret = i;

    out:
    return ret;
}

s32 fat16_get_root_directory(disk_t *disk, fat_private_t *fat_private, fat_directory_t *directory)
{
    kerr_no_t  ret = kerr_OK;
    fat_header_t *primary_header = &fat_private->header.primary_header;

    s32 root_dir_sector_pos = (primary_header->fat_copies * primary_header->sectors_per_fat) + primary_header->reserved_sectors;
    s32 root_dir_entries = fat_private->header.primary_header.root_dir_entries;
    s32 root_dir_size = (root_dir_entries * sizeof(fat_directory_item_t));
    s32 total_sectors = root_dir_size / disk->sector_size;

    /*
     * if our storage bytes are 600 bytes, and one sector is 512 bytes
     * we need to sectors to store our data
     * */
    if(root_dir_size % disk->sector_size)
    {
        total_sectors += 1;
    }

    s32 total_items = fat16_get_total_items_for_directory(disk, root_dir_sector_pos);

    fat_directory_item_t *dir = kzalloc(root_dir_size);
    if(dir == NULL)
    {
        ret = -kerr_NOMEM;
        goto out;
    }

    ds_t *stream = fat_private->directory_stream;
    if(disk_streamer_seek(stream, fat16_sector_to_absolute(disk, root_dir_sector_pos)) != kerr_OK)
    {
        ret = -kerr_IO;
        goto out;
    }

    if(disk_streamer_read(stream, dir, root_dir_size) != kerr_OK)
    {
        ret = -kerr_IO;
        goto out;
    }

    directory->item = dir;
    directory->total = total_items;
    directory->sector_pos = root_dir_sector_pos;
    directory->ending_sector_pos = root_dir_sector_pos + (root_dir_size / disk->sector_size);

    out:
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
    if(stream)
    {
        disk_streamer_close(stream);
    }

    if(ret < 0)
    {
        kfree(fat_private);
        disk->fs_private = 0;
    }

    return ret;
}