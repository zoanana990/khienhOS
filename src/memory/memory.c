#include <memory/memory.h>

void* memset(void *ptr, i32 c, size_t size)
{
    c8* c_ptr = (c8*) ptr;
    for(i32 i = 0; i < size; i++)
    {
        c_ptr[i] = (c8) c;
    }
    return ptr;
}
