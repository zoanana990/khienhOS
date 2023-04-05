#ifndef KHIENHOS_STRING_H
#define KHIENHOS_STRING_H

#include <khienh/type.h>

s32 strlen(const s8 *);
s32 strnlen(const s8 *, s32);
s8 *strcpy(s8 *dst, const s8 *src);
bool_t is_digit(s8 c);
s32 to_numeric_digit(s8 c);

#endif //KHIENHOS_STRING_H
