#ifndef KHIENHOS_PRINT_H
#define KHIENHOS_PRINT_H

#include <khienh/type.h>

/* TODO: do all printf function in print */
void print(const c8 *str);

/* TODO: debug log */
typedef enum
{
    log_error_msg = 0,
    log_warning_msg,
    log_info_msg,
    log_last
} debug_level_t;

#define LOG

#endif //KHIENHOS_PRINT_H
