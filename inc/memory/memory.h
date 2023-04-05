#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <khienh/type.h>


void *memset(void *ptr, s32 c, size_t size);
void *memcpy(void *dst, const void *src, size_t len);
s32 memcmp(const void *str1, const void *str2, size_t count);


#endif
