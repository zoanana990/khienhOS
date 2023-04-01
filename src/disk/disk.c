#include <disk/disk.h>

disk_t disk;

/**
 * Reference the LBA register:
 * https://www.cnblogs.com/jadeshu/p/11873540.html
 *
 * LBA address 28 bit:
 *
 * high -----------------------> low
 * | 4 bit | 8 bit | 8 bit | 8 bit |
 * | 0x1f6 | 0x1f5 | 0x1f4 | 0x1f3 |
 * */
static s32 disk_read_sector(s32 lba, s32 total, void *buf)
{
    /**
     * 0x1f6 is device registers
     * BIT[0-3]: lba address high 4 bit
     * BIT[4]: major disk is 0
     * BIT[5]: must be 1
     * BIT[6]: 1 for LBA mode, 0 for CHS mode
     * BIT[7]: must be 1
     * */
    outb(0x1f6, (lba >> 24) | 0xe0);

    /**
     * 0x1f2 is used to assign the total amount of read/write sector
     * */
    outb(0x1f2, total);

    /**
     * 0x1f3 is the LBA address low 8 bits
     * 0x1f4 is the LBA address medium 8 bits
     * 0x1f5 is the LBA address high 8 bits
     * */
    outb(0x1f3, (u8)(lba & 0xff));
    outb(0x1f4, (u8)(lba >> 8));
    outb(0x1f5, (u8)(lba >> 16));

    /**
     * in 0x1f7 register,
     * 0x20 denotes for read the sector
     * 0x30 denotes for write the sector
     * 0xec denotes for identify the sector
     * */
    outb(0x1f7, 0x20);

    u16 *ptr = (u16 *) buf;
    for(s32 j = 0; j < total; j++)
    {
        /* wait for the bus to be ready */
        s8 c = insb(0x1f7);
        while(!(c & 0x08))
        {
            c = insb(0x1f7);
        }

        /* Copy from hard disk to memory */
        for(s32 i = 0; i < 256; i++)
        {
            *ptr = insw(0x1f0);
            ptr++;
        }
    }

    return 0;
}

void disk_search_and_init()
{
    memset(&disk, 0, sizeof(disk_t));
    disk.type = KHIENHOS_DISK_TYPE_REAL;
    disk.sector_size = KHIENHOS_DISK_SECTOR_SIZE;
}

disk_t *disk_get(s32 index)
{
    if(index != 0)
        return NULL;
    return &disk;
}

s32 disk_read_block(disk_t *local_disk, u32 lba, s32 total, void *buf)
{
    if(local_disk != &disk)
        return -kerr_IO;

    return disk_read_sector(lba, total, buf);
}