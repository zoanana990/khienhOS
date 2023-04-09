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

/* @function: strlen_terminator
 * @description:
 * find the terminator or the '\0'
 */
s32 strlen_terminator(const s8 *str, s32 max, s8 terminator)
{
    s32 index = 0;
    for(index = 0; index < max; index++)
    {
        if(str[index] == '\0' || str[index] == terminator)
            break;
    }
    return index;
}

/* turn the character to the lower case */
s8 tolower(s8 s)
{
    /* check the ASCII table */
    if(s >= 'A' && s <= 'Z')
    {
        s += 32;
    }
    return s;
}

s32 istrncmp(const s8 *s1, const s8 *s2, s32 n)
{
    register u8 u1, u2;

    while(n-- > 0)
    {
        u1 = (u8) *s1++;
        u2 = (u8) *s2++;
        if(u1 != u2 && tolower(u1) != tolower(u2))
            return u1 - u2;
        if(u1 == '\0')
            return 0;
    }
    return 0;
}

/* @function: strncmp
 * @description: compare two string, 
 * if s1 == s2, return 0
 * else if s1 > s2, return positive number
 * else return negative number
 */
s32 strncmp(const s8 *s1, const s8 *s2, s32 n)
{
    register u8 u1, u2;

    while(n-- > 0)
    {
        u1 = (u8) *s1++;
        u2 = (u8) *s2++;
        if(u1 != u2)
            return u1 - u2;
        if(u1 == '\0')
            return 0;
    }
    return 0;
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