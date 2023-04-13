#include <khienh/print.h>

static u16 *video_mem;
static u16 console_row = 0;
static u16 console_col = 0;

/*
 * This is a simple and clear design
 * the number index is corresponding for the mod value
 * for example:
 * '0' is corresponding the number % n == 0
 * */
static s8  digits[] = "0123456789abcdef";

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

static void print_integer(s32 xx, s32 base, s32 sign)
{
    s8 buf[16];
    s32 i;
    u32 x;

    if(sign && (sign = xx < 0))
    {
        x = -xx;
    }
    else
    {
        x = xx;
    }

    i = 0;
    do{
        buf[i++] = digits[x % base];
    }while((x /= base) != 0);

    if(sign)
        buf[i++] = '-';
    while(i-- >= 0)
        console_write_char(buf[i], FC_TEXT);
}

/* u64: xx
 * sizeof(u64) * 8 - 4 = 16 * 8 - 4
 * */
static void print_pointer(u32 xx)
{
    s32 i;
    console_write_char('0', FC_TEXT);
    console_write_char('x', FC_TEXT);
    for(i = 0; i < (sizeof(u32) * 2); i++, xx <<= 4)
    {
        console_write_char(digits[xx >> (sizeof(u32) * 8 - 4)], FC_TEXT);
    }
}

/* print to the console, only print %d, %x, %p, %s, %c */
void print(s8 *str, ...)
{
    va_list va;

    s32 i, c;
    s8 *s;

    va_start(va, str);
    for(i = 0; (c = str[i] & 0xff) != 0; i++)
    {
        /* if c is not %, just print the character */
        if(c != '%')
        {
            console_write_char(c, FC_TEXT);
            continue;
        }

        /* if so, consider the following condition
         * there is no character, print '%'
         * d: %d, print the integer
         * s: %s, print the string
         * x: %x, print the integer based on 16
         * p: %p, print the pointer address
         * */
        c = str[++i] & 0xff;
        if(c == 0)
        {
            break;
        }

        switch(c)
        {
            case 'd':
                print_integer(va_arg(va, s32), 10, 1);
                break;
            case 'x':
                print_integer(va_arg(va, s32), 16, 1);
                break;
            case 'p':
                print_pointer(va_arg(va, u32));
                break;
            case 's':
                if((s = va_arg(va, s8 *)) == 0)
                    s = "(NULL)";
                for(; *s; s++)
                    console_write_char(*s, FC_TEXT);
                break;
            case '%':
                console_write_char('%', FC_TEXT);
                break;
            default:
                console_write_char('%', FC_TEXT);
                console_write_char(c, FC_TEXT);
                break;
        }
        va_end(va);
    }
}

/* not really realize the va_... macros */
void print_dbg(s8 *str, ...)
{
    va_list args;
    va_start(args, str);
    print("[%s]: %s",__func__, args);
    va_end(args);
}

/* TODO need to identify the message by using different color */