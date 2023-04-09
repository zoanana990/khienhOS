#ifndef KHIENHOS_STRING_H
#define KHIENHOS_STRING_H

#include <khienh/type.h>

s32 strlen(const s8 *);
s32 strnlen(const s8 *, s32);
s32 strncmp(const s8 *s1, const s8 *s2, s32 n);
s32 istrncmp(const s8 *s1, const s8 *s2, s32 n);
s32 strlen_terminator(const s8 *str, s32 max, s8 terminator);
s8 tolower(s8 s);
s8 *strcpy(s8 *dst, const s8 *src);
bool_t is_digit(s8 c);
s32 to_numeric_digit(s8 c);

#endif //KHIENHOS_STRING_H
