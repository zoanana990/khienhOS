#include <memory/heap.h>
#include <memory/memory.h>

static kerr_no_t heap_validate_table(void *ptr, void *end, heap_table_t *table)
{
    kerr_no_t ret = kerr_OK;

    size_t table_size = (size_t)(end - ptr);
    size_t total_blocks = table_size / KHIENHOS_HEAP_BLOCK_SIZE;

    if(table->total != total_blocks)
    {
        ret = kerr_INVARG;
        goto out;
    }

out:
    return ret;
}

static bool_t heap_is_alignment(void *ptr)
{
    return ((u32) ptr % KHIENHOS_HEAP_BLOCK_SIZE) == 0;
}

static u32 heap_alloc_value_align(u32 val)
{
    if(val % KHIENHOS_HEAP_BLOCK_SIZE == 0)
    {
        return val;
    }

    /* * description
     * align size to upper value
     * val = 5000 -> 5000 - 5000 % 4096 + 4096 = 5000 - 4 + 4096 = 8192
     * */
    return val - (val % KHIENHOS_HEAP_BLOCK_SIZE) + KHIENHOS_HEAP_BLOCK_SIZE;
}

kerr_no_t heap_create(heap_t *heap, void *ptr, void *end, heap_table_t *table)
{
    kerr_no_t ret = kerr_OK;

    if(!heap_is_alignment(ptr) || !heap_is_alignment(end))
    {
        ret = kerr_INVARG;
        goto out;
    }

    memset(heap, 0, sizeof(heap_t));
    heap->saddr = ptr;
    heap->table = table;

    ret = heap_validate_table(ptr, end, table);

    if(ret)
    {
        goto out;
    }

    /* initialize the heap region to free */
    size_t table_size = sizeof(heap_block_entry_t) * table->total;
    memset(table->entry, HEAP_BLOCK_IS_FREE, table_size);

out:
    return ret;
}



void *heap_malloc(size_t size)
{
    size_t aligned_size = heap_alloc_value_align(size);
    u32 total_blocks = aligned_size / KHIENHOS_HEAP_BLOCK_SIZE;

    return 0;
}

void heap_free(void *ptr)
{
    return;
}