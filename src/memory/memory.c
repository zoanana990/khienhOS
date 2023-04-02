#include <memory/memory.h>

void* memset(void *ptr, s32 c, size_t size)
{
    s8* c_ptr = (s8*) ptr;
    for(s32 i = 0; i < size; i++)
    {
        c_ptr[i] = (s8) c;
    }
    return ptr;
}

s32 memcmp (const void *str1, const void *str2, size_t count)
{
    register const u8 *s1 = (const u8 *)str1;
    register const u8 *s2 = (const u8 *)str2;

    while (count-- > 0)
    {
        if (*s1++ != *s2++)
            return s1[-1] < s2[-1] ? -1 : 1;
    }
    return 0;
}
