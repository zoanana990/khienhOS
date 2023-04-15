#ifndef KHIENHOS_PRINT_H
#define KHIENHOS_PRINT_H

#include <khienh/type.h>
#include <khienh/font_color.h>
#include <common/string.h>

/*  definition of the screen */
#define VGA_WIDTH 80
#define VGA_HEIGHT 20

/* TODO : debug log */
typedef enum
{
    log_error_msg = 0,
    log_warning_msg,
    log_info_msg,
    log_last
} debug_level_t;

/* TODO : do all printf function in print */
void print(s8 *str, ...);



/**
 * @func: console_init
 * @description:
 *      Write the screen register 0xB800 to put the character and the color out.
 * */
void console_init();

#define LOG(fmt, args...) print("\r[%s]:" fmt, __func__, ##args)

#endif //KHIENHOS_PRINT_H
