#include <khienh/kernel.h>
#include <khienh/type.h>
#include <khienh/font_color.h>

u16 *video_mem;

/* put character, and we can assign the color for the character */
u16 put_char(c8 c, c8 color)
{
    return (color << 8) | c;
}

/* put character on the specified position */
void console_put_char(i32 x, i32 y, c8 c, c8 color)
{
    video_mem[(y * VGA_WIDTH) + x] = put_char(c, color);
}

void console_init()
{
    video_mem = (u16*)(0xB8000);
    for(i32 y = 0; y < VGA_HEIGHT; y++)
    {
        for(i32 x = 0; x < VGA_WIDTH; x++)
        {
            video_mem[(y * VGA_WIDTH) + x] = put_char(' ', 0);
        }
    }
}

void kernel_main()
{
    console_init();
    video_mem[0] = put_char('A', FC_YELLOW);
}