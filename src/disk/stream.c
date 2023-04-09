#include <disk/stream.h>
#include <memory/kheap.h>

ds_t *disk_streamer_new(s32 disk_id)
{
    disk_t *disk = disk_get(disk_id);
    if(!disk)
    {
        return NULL;
    }

    ds_t *streamer = kzalloc(sizeof(ds_t));
    streamer->pos = 0;
    streamer->disk = disk;

    return streamer;
}

s32 disk_streamer_seek(ds_t *stream, s32 pos)
{
    stream->pos = pos;
    return 0;
}

s32 disk_streamer_read(ds_t *stream, void *out, s32 total)
{
    /* disk sector and offset */
    s32 sector = stream->pos / KHIENHOS_DISK_SECTOR_SIZE;
    s32 offset = stream->pos % KHIENHOS_DISK_SECTOR_SIZE;
    s8  buf[KHIENHOS_DISK_SECTOR_SIZE];

    s32 ret = disk_read_block(stream->disk, sector, 1, buf);
    if(ret < 0)
    {
        goto out;
    }

    s32 total_to_read = total > KHIENHOS_DISK_SECTOR_SIZE ? KHIENHOS_DISK_SECTOR_SIZE : total;
    for(s32 i = 0; i < total_to_read; i++)
    {
        *(s8 *) out++ = buf[offset + i];
    }

    stream->pos += total_to_read;
    if(total > KHIENHOS_DISK_SECTOR_SIZE)
    {
        ret = disk_streamer_read(stream, out, total - KHIENHOS_DISK_SECTOR_SIZE);
    }

    out:
    return ret;
}

void disk_streamer_close(ds_t *stream)
{
    kfree(stream);
}