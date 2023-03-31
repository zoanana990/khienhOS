#ifndef KHIENHOS_HEAP_H
#define KHIENHOS_HEAP_H

#include <khienh/config.h>
#include <khienh/type.h>

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN  0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE   0x00

#define HEAP_BLOCK_HAS_NEXT           0b10000000
#define HEAP_BLOCK_IS_FREE            0b1000000

typedef u8 heap_block_entry_t;

typedef struct heap_table
{
    heap_block_entry_t *entry;
    size_t             total;
} heap_table_t;

typedef struct heap
{
    heap_table_t  *table;
    void          *saddr; /* start address of the heap data pool */
} heap_t;

kerr_no_t heap_create(heap_t *heap, void *ptr, void *end, heap_table_t *table);
void *heap_malloc(size_t size);
void heap_free(void *ptr);

#endif // KHIENHOS_HEAP_H
