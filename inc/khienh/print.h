#ifndef KHIENHOS_PRINT_H
#define KHIENHOS_PRINT_H

#include <khienh/type.h>
#include <khienh/font_color.h>

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

void console_init();

/* TODO : do all printf function in print */
void print(const s8 *str, ...);

#endif //KHIENHOS_PRINT_H
