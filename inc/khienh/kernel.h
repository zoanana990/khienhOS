#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <khienh/type.h>
#include <khienh/print.h>
#include <common/string.h>

#define KHIENHOS_MAX_PATH 256


#define ERROR(value) (void *)(value)
#define ERROR_I(value) ((s32)(value))
#define IS_ERR(value) ((s32) value < 0)

void kernel_main();

#endif