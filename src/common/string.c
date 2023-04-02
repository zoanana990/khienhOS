#include <common/string.h>

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

