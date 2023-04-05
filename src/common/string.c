#include <common/string.h>
#include <memory/memory.h>

s32 strlen(const s8 *ptr)
{
    s32 i = 0;
    while(*ptr != 0)
    {
        i++;
        ptr += 1;
    }

    return i;
}

s32 strnlen(const s8 *ptr, s32 max)
{
    s32 i = 0;
    for(; i < max; i++)
    {
        if(ptr[i] == 0)
            break;
    }

    return i;
}

bool_t is_digit(s8 c)
{
    return c >= 47 && c <= 58;
}
s32 to_numeric_digit(s8 c)
{
    return c - 48;
}

s8 *strcpy(s8 *dst, const s8 *src)
{
    return memcpy(dst, src, strlen(src) + 1);
}