#include <khienh/kernel.h>
#include <khienh/type.h>
#include <khienh/font_color.h>
#include <memory/kheap.h>
#include <idt/idt.h>
#include <io/io.h>

static u16 *video_mem;
static u16 console_row = 0;
static u16 console_col = 0;

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

void console_write_char(c8 c, c8 color)
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
    for(i32 y = 0; y < VGA_HEIGHT; y++)
    {
        for(i32 x = 0; x < VGA_WIDTH; x++)
        {
            video_mem[(y * VGA_WIDTH) + x] = put_char(' ', 0);
        }
    }
}

size_t strlen(const c8* str)
{
    size_t len = 0;
    while(str[len++]);
    return len-1;
}

void print(const c8 *str, ...)
{
    size_t len = strlen(str);
    for(i32 i = 0; i < len; i++)
    {
        console_write_char(str[i], FC_TEXT);
    }
}

void kernel_main()
{
    console_init();
    print("Hello World\n");

    /* Initialize the idt */
    idt_init();

    /* initialize the heap */
    kheap_init();

    /* enable interrupt */
    enable_interrupt();
}