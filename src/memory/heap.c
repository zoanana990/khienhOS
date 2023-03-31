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

static int heap_get_entry_type(heap_block_entry_t entry)
{
    return entry & 0xf;
}

/*
 * return
 * <0 : error occur
 * */
i32 heap_get_start_block(heap_t *heap, u32 total_blocks)
{
    heap_table_t *table = heap->table;

    /* current block */
    int bc = 0;

    /* previous block */
    int bs = -1;

    /* traverse all the heap region */
    for(size_t i = 0; i < table->total; i++)
    {
        if(heap_get_entry_type(table->entry[i]) != HEAP_BLOCK_TABLE_ENTRY_FREE)
        {
            bc = 0, bs = -1;
            continue;
        }

        if(bs == -1)
            bs = i;

        bc++;

        if(bc == total_blocks)
            break;
    }

    if(bs == -1)
        return -1;

    return bs;
}

void *heap_block_to_address(heap_t *heap, u32 block)
{
    return heap->saddr + (block * KHIENHOS_HEAP_BLOCK_SIZE);
}

void heap_mask_blocks_taken(heap_t *heap, i32 start_block, i32 total_blocks)
{
    i32 end_block = start_block + total_blocks - 1;

    heap_block_entry_t entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
    if(total_blocks > 1)
    {
        entry |= HEAP_BLOCK_HAS_NEXT;
    }

    for(i32 i = 0; i <= end_block; i++)
    {
        heap->table->entry[i] = entry;
        entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
        if(i != end_block - 1)
        {
            entry |= HEAP_BLOCK_HAS_NEXT;
        }
    }
}

/* malloc the block here and return a pointer in the data pool */
void *heap_malloc_block(heap_t *heap, u32 total_blocks)
{
    void *address = 0;

    i32 start_block = heap_get_start_block(heap, total_blocks);

    if(start_block < 0)
        goto out;

    address = heap_block_to_address(heap, start_block);

    /* mark the block is taken */
    heap_mask_blocks_taken(heap, start_block, total_blocks);

    out:
    return address;
}

void *heap_malloc(heap_t *heap, size_t size)
{
    size_t aligned_size = heap_alloc_value_align(size);
    u32 total_blocks = aligned_size / KHIENHOS_HEAP_BLOCK_SIZE;
    return heap_malloc_block(heap, total_blocks);
}

void heap_free_block(heap_t heap, u32 starting_block)
{

}

void heap_free(heap_t *heap, void *ptr)
{
    return;
}