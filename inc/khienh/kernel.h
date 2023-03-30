#ifndef _KERNEL_H_
#define _KERNEL_H_

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

#include <khienh/type.h>

void kernel_main();
void print(const c8 *str);

#endif