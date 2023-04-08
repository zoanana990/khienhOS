#include <khienh/print.h>
#include <common/string.h>

static u16 *video_mem;
static u16 console_row = 0;
static u16 console_col = 0;

/* put character, and we can assign the color for the character */
static u16 put_char(s8 c, s8 color)
{
    return (color << 8) | c;
}

/* put character on the specified position */
static void console_put_char(s32 x, s32 y, s8 c, s8 color)
{
    video_mem[(y * VGA_WIDTH) + x] = put_char(c, color);
}

static void console_write_char(s8 c, s8 color)
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
