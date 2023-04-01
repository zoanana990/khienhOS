#include <memory/kheap.h>

heap_t kernel_heap;
heap_table_t kernel_heap_table;

/* initialize the heap region */
void kheap_init()
{
    u32 total_table_entry = (u32) KHIENHOS_HEAP_MAX_SIZE / KHIENHOS_HEAP_BLOCK_SIZE;
    kernel_heap_table.entry = (heap_block_entry_t *)(KHIENHOS_HEAP_TABLE_ADDR);
    kernel_heap_table.total = total_table_entry;

    void *start = (void *) KHIENHOS_HEAP_START_ADDR;
    void *end = (void *)(KHIENHOS_HEAP_START_ADDR + KHIENHOS_HEAP_MAX_SIZE);
    kerr_no_t ret = heap_create(&kernel_heap, start, end, &kernel_heap_table);
    if(ret)
    {
        print("Failed to create heap\n");
    }
}

void *kmalloc(size_t size)
{
    return heap_malloc(&kernel_heap, size);
}

/* allocate memory and initialize the region to zero */
void *kzalloc(size_t size)
{
    void *ptr = kmalloc(size);

    if(!ptr) return NULL;

    memset(ptr, 0x00, size);
    return ptr;
}

void kfree(void *ptr)
{
    heap_free(&kernel_heap, ptr);
}