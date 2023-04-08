#ifdef COSTOM_PRINT

#include <khienh/print.h>

/**
 * Tiny printk implementation
 * You have to implement _putchar if you use printf()
 * To avoid conflicts with the regular printf() API it is overridden by macro defines
 * and internal underscore-appended functions like printf_() are used
 * \param format A string that specifies the format of the output
 * \return The number of characters that are written into the array, not counting the terminating null character
 */
s32 printk(const s8* format, ...)
{
    return 0;
}

s32 sprintk(s8 *buffer, const s8* format, ...)
{
    return 0;
}

#endif