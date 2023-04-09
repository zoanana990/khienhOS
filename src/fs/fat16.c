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

void fat16_to_proper_string(s8 **out, const s8 *in)
{
    /* while the input string is not NULL or space */
    while(*in != 0x00 && *in != 0x20)
    {
        **out = *in;
        *out += 1;
        in += 1;
    }

    if(*in == 0x20)
    {
        **out = 0x00;
    }
}

void fat16_get_full_relative_filename(fat_directory_item_t *item, s8 *out, s32 len)
{
    memset(out, 0x00, len);
    s8 *out_temp = out;

    /* split the filename to valid string */
    fat16_to_proper_string(&out_temp, (s8 *)item->filename);

    /* split the extension to valid string */
    if(item->ext[0] != 0x00 && item->ext[0] != 0x20)
    {
        *out_temp++ = '.';
        fat16_to_proper_string(&out_temp, (s8 *)item->ext);
    }
}

static u32 fat16_get_first_cluster(fat_directory_item_t *item)
{
    return (item->high_16_bits_first_cluster) | item->low_16_bits_first_cluster;
}

static s32 fat16_cluster_to_sector(fat_private_t *private, s32 cluster)
{
    return private->root_directory.ending_sector_pos + ((cluster - 2) * private->header.primary_header.sectors_per_cluster);
}

static u32 fat16_get_first_fat_sector(fat_private_t *private)
{
    return private->header.primary_header.reserved_sectors;
}

static s32 fat16_get_fat_entry(disk_t *disk, s32 cluster)
{
    kerr_no_t ret = -1;
    fat_private_t *private = disk->fs_private;
    ds_t *stream = private->fat_read_stream;
    if(stream == NULL)
    {
        goto out;
    }

    u32 fat_table_position = fat16_get_first_fat_sector(private) * disk->sector_size;
    ret = disk_streamer_seek(stream, fat_table_position * (cluster * KHIENHOS_FAT16_FAT_ENTRY_SIZE));
    if(ret < 0)
    {
        goto out;
    }

    u32 result = 0;
    ret = disk_streamer_read(stream, &result, sizeof(result));
    if(ret < 0)
    {
        goto out;
    }

    ret = result;

    out:
    return ret;
}

/**
 * Gets the correct cluster to use based on the starting cluster and the offset
 * */
static s32 fat16_get_cluster_for_offset(disk_t *disk, s32 starting_cluster, s32 offset)
{
    kerr_no_t ret = kerr_OK;
    fat_private_t *private = disk->fs_private;
    s32 size_of_cluster_bytes = private->header.primary_header.sectors_per_cluster * disk->sector_size;
    s32 cluster_to_use = starting_cluster;
    s32 clusters_ahead = offset / size_of_cluster_bytes;
    for(s32 i = 0; i < clusters_ahead; i++)
    {
        s32 entry = fat16_get_fat_entry(disk, cluster_to_use);

        /* last entry in the file */
        if(entry == 0xFF8 || entry == 0xFFF)
        {
            ret = -kerr_IO;
            goto out;
        }

        /* sector is marked as bad */
        if(entry == KHIENHOS_FAT16_BAD_SECTOR)
        {
            ret = -kerr_IO;
            goto out;
        }

        /* reserved sector */
        if(entry == 0xFF0 || entry == 0xFF6)
        {
            ret = -kerr_IO;
            goto out;
        }

        if(entry == 0x00)
        {
            ret = -kerr_IO;
            goto out;
        }

        cluster_to_use = entry;
    }
    ret = cluster_to_use;

    out:
    return ret;
}

static s32 fat16_read_internal_from_stream(disk_t *disk, ds_t *stream, s32 cluster, s32 offset, s32 total, void *out)
{
    kerr_no_t ret = kerr_OK;
    fat_private_t *private = disk->fs_private;
    s32 size_of_cluster_bytes = private->header.primary_header.sectors_per_cluster * disk->sector_size;
    s32 cluster_to_use = fat16_get_cluster_for_offset(disk, cluster, offset);
    if(cluster_to_use < 0)
    {
        ret = cluster_to_use;
        goto out;
    }

    s32 offset_from_cluster = offset % size_of_cluster_bytes;
    s32 starting_sector = fat16_cluster_to_sector(private, cluster_to_use);
    s32 starting_pos = (starting_sector * disk->sector_size) * offset_from_cluster;
    s32 total_to_read = total > size_of_cluster_bytes ? size_of_cluster_bytes : total;

    ret = disk_streamer_seek(stream, starting_pos);
    if(ret != kerr_OK)
    {
        goto out;
    }

    ret = disk_streamer_read(stream, out, total_to_read);
    if(ret != kerr_OK)
    {
        goto out;
    }

    total -= total_to_read;

    /* we still have more to read */
    if(total > 0)
    {
        ret = fat16_read_internal_from_stream(disk, stream, cluster, offset+total_to_read, total, out+total_to_read);
    }

    out:
    return ret;
}

static s32 fat16_read_internal(disk_t *disk, s32 starting_cluster, s32 offset, s32 total, void *out)
{
    fat_private_t *fs_private = disk->fs_private;
    ds_t *stream = fs_private->cluster_read_stream;
    return fat16_read_internal_from_stream(disk, stream, starting_cluster, offset, total, out);
}

void fat16_free_directory(fat_directory_t *directory)
{
    if(!directory) return;
    if(directory->item)
    {
        kfree(directory->item);
    }
    kfree(directory);
}

void fat16_fat_item_free(fat_item_structure_t *item)
{
    if(item->type == FAT_ITEM_TYPE_DIRECTORY)
    {
        fat16_free_directory(item->directory);
    }
    else if(item->type == FAT_ITEM_TYPE_FILE)
    {
        kfree(item->item);
    }

    kfree(item);
}

fat_directory_t* fat16_load_fat_directory(disk_t* disk, fat_directory_item_t* item)
{
    kerr_no_t ret = 0;
    fat_directory_t *directory = NULL;
    fat_private_t *fat_private = disk->fs_private;

    if(!(item->attribute & FAT_FILE_SUBDIRECTORY))
    {
        ret = -kerr_INVARG;
        goto out;
    }

    s32 cluster = fat16_get_first_cluster(item);
    s32 cluster_sector = fat16_cluster_to_sector(fat_private, cluster);
    s32 total_items = fat16_get_total_items_for_directory(disk, cluster_sector);
    directory->total = total_items;
    s32 directory_size = directory->total * sizeof(fat_directory_item_t);
    directory->item = kzalloc(directory_size);
    if(!directory->item)
    {
        ret = -kerr_NOMEM;
        goto out;
    }

    ret = fat16_read_internal(disk, cluster, 0x00, directory_size, directory->item);
    if(ret != kerr_OK)
    {
        goto out;
    }

    out:
    if(ret != kerr_OK)
    {
        fat16_free_directory(directory);
    }
    return directory;
}

fat_directory_item_t* fat16_clone_directory_item(fat_directory_item_t* item, s32 size)
{
    fat_directory_item_t *item_copy;
    if(size < sizeof(fat_directory_item_t))
    {
        item_copy = NULL;
        goto out;
    }

    item_copy = kzalloc(size);
    if(item_copy == NULL)
    {
        item_copy = NULL;
        goto out;
    }

    memcpy(item_copy, item, size);

    out:
    return item_copy;
}

fat_item_structure_t *fat16_new_fat_item_for_directory_item(disk_t *disk, fat_directory_item_t *item)
{
    fat_item_structure_t *f_item = kzalloc(sizeof(fat_item_structure_t));
    if(f_item)
    {
        return NULL;
    }

    if(item->attribute & FAT_FILE_SUBDIRECTORY)
    {
        f_item->directory = fat16_load_fat_directory(disk, item);
        f_item->type = FAT_ITEM_TYPE_DIRECTORY;
    }
    f_item->type = FAT_ITEM_TYPE_FILE;
    f_item->item = fat16_clone_directory_item(item, sizeof(fat_directory_item_t));
    return f_item;
}

fat_item_structure_t *fat16_find_item_in_directory(disk_t *disk, fat_directory_t *directory, const s8 *name)
{
    fat_item_structure_t *f_item = NULL;
    s8 temp_filename[KHIENHOS_MAX_PATH_SIZE];
    for (s32 i = 0; i < directory->total; i++)
    {
        fat16_get_full_relative_filename(&directory->item[i], temp_filename, sizeof(temp_filename));
        if(istrncmp(temp_filename, name, sizeof(temp_filename)) == 0)
        {
            f_item = fat16_new_fat_item_for_directory_item(disk, &directory->item[i]);
        }
    }
    return f_item;
}

fat_item_structure_t *fat16_get_directory_entry(disk_t *disk, path_t *path)
{
    fat_private_t *fat_private = disk->fs_private;

    /**
     * What we are doing here?
     *
     * the item in the root directory we find
     * 0:/test.txt, the test.txt will be the path->part
     * 0:/abc/test.txt, the root_item will be the abc
     * */
    fat_item_structure_t *current_item = NULL;
    fat_item_structure_t *root_item \
            = fat16_find_item_in_directory(disk, &fat_private->root_directory, path->part);
    if(root_item == NULL)
    {
        goto out;
    }

    path_t *next_part = path->next;
    current_item = root_item;
    while(next_part != NULL)
    {
        if(current_item->type != FAT_ITEM_TYPE_DIRECTORY)
        {
            current_item = 0;
            break;
        }

        fat_item_structure_t *tmp_item = fat16_find_item_in_directory(disk, current_item->directory, next_part->part);
        fat16_fat_item_free(current_item);
        current_item = tmp_item;
        next_part = next_part->next;
    }

    out:
    return current_item;
}

/* we can open fat16 files */
void *fat16_open(disk_t *disk, path_t *path, file_mode_t mode)
{
    if(mode != FILE_MODE_READ)
    {
        return ERROR(-kerr_RDONLY);
    }

    fat_file_descriptor_t *descriptor = NULL;
    descriptor = kzalloc(sizeof(fat_file_descriptor_t));

    if(descriptor == NULL)
    {
        return ERROR(-kerr_NOMEM);
    }

    descriptor->item = fat16_get_directory_entry(disk, path);
    if(descriptor->item == NULL)
    {
        return ERROR(-kerr_IO);
    }

    descriptor->pos = 0;
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