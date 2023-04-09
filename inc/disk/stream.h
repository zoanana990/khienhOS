#ifndef KHIENHOS_STREAM_H
#define KHIENHOS_STREAM_H

#include <disk/disk.h>
#include <khienh/type.h>

typedef struct disk_stream
{
    s32 pos;
    disk_t *disk;
} ds_t;

ds_t *disk_streamer_new(s32 disk_id);
s32 disk_streamer_seek(ds_t *stream, s32 pos);
s32 disk_streamer_read(ds_t *stream, void *out, s32 total);
void disk_streamer_close(ds_t *stream);

#endif //KHIENHOS_STREAM_H
