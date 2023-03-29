#include <khienh/kernel.h>
#include <khienh/type.h>

/* put character, and we can assign the color for the character */
u16 put_char(c8 c, c8 color)
{
    return (color << 8) | c;
}

void kernel_main()
{
    u16 *video_mem = (u16*)(0xB8000);
    video_mem[0] = put_char('A', 3);
}