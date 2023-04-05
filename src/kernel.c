#include <khienh/kernel.h>
#include <khienh/type.h>
#include <khienh/font_color.h>
#include <memory/kheap.h>
#include <memory/page.h>
#include <idt/idt.h>
#include <common/string.h>
#include <fs/file.h>
#include <disk/stream.h>

static u16 *video_mem;
static u16 console_row = 0;
static u16 console_col = 0;

/* put character, and we can assign the color for the character */
u16 put_char(s8 c, s8 color)
{
    return (color << 8) | c;
}

/* put character on the specified position */
void console_put_char(s32 x, s32 y, s8 c, s8 color)
{
    video_mem[(y * VGA_WIDTH) + x] = put_char(c, color);
}

void console_write_char(s8 c, s8 color)
{
    if(c == '\n')
    {
        console_col = 0;
        console_row += 1;
        return ;
    }
    console_put_char(console_col, console_row, c, color);
    console_col += 1;
    if (console_col >= VGA_WIDTH)
    {
        console_col = 0;
        console_row += 1;
    }
}

void console_init()
{
    video_mem = (u16*)(0xB8000);
    for(s32 y = 0; y < VGA_HEIGHT; y++)
    {
        for(s32 x = 0; x < VGA_WIDTH; x++)
        {
            video_mem[(y * VGA_WIDTH) + x] = put_char(' ', 0);
        }
    }
}

void print(const s8 *str, ...)
{
    va_list va;
    va_start(va, str);

    /* old version for print functions */
    size_t len = strlen(str);
    for(s32 i = 0; i < len; i++)
    {
        console_write_char(str[i], FC_TEXT);
    }

    va_end(va);
}

paging_4gb_chunk_t *kernel_chunk = 0;

void kernel_main()
{
    console_init();
    print("Hello World\n");

    /* Initialize the idt */
    idt_init();

    /* initialize the heap */
    kheap_init();

    /* initialize the file system */
    fs_init();

    /* Setup paging */
    kernel_chunk = paging_new_4gb(PAGING_WRITE_PRESENT_ACCESS);

    /* switch to kernel paging chunk */
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));

    /* Enable paging */
    enable_paging();

    /* enable interrupt */
    enable_interrupt();

    ds_t* stream = disk_streamer_new(0);
    disk_streamer_seek(stream, 0x201);
    unsigned char c = 0;
    disk_streamer_read(stream, &c, 1);
    while(1) {}
}