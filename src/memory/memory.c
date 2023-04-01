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
