#include <memory/page.h>
#include <memory/kheap.h>
#include <khienh/print.h>

/********************************
 * local global variable
 ********************************/
static u32* current_directory = 0;

/********************************
 * local function
 ********************************/
void paging_load_directory(u32 *directory);

/********************************
 * export function
 ********************************/
paging_4gb_chunk_t *paging_new_4gb(u8 flags)
{
    u32 *directory = kzalloc(sizeof(u32) * PAGING_TOTAL_ENTRIES_PER_TABLE);
    s32 offset = 0;

    for(s32 i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++)
    {
        u32 *entry = kzalloc(sizeof(u32) * PAGING_TOTAL_ENTRIES_PER_TABLE);
        for(s32 j = 0; j < PAGING_TOTAL_ENTRIES_PER_TABLE; j++)
        {
            entry[j] = (offset + (j * PAGING_SIZE)) | flags;
        }
        offset += PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_SIZE;
        directory[i] = (u32)entry | flags | PAGING_IS_WRITABLE;
    }

    paging_4gb_chunk_t  *chunk_4gb = kzalloc(sizeof(paging_4gb_chunk_t));
    chunk_4gb->directory_entry = directory;

    return chunk_4gb;
}

void paging_switch(u32 *directory)
{
    paging_load_directory(directory);
    current_directory = directory;
}

u32 *paging_4gb_chunk_get_directory(paging_4gb_chunk_t *chunk)
{
    return chunk->directory_entry;
}

bool_t paging_is_aligned(void *address)
{
    return ((u32) address % PAGING_SIZE) == 0;
}

/** paging_get_indexes
 * \return minus    -> error
 *         positive -> address
 * */
s32 paging_get_indexes(void *virtual_address, u32 *directory_index_out, u32* table_index_out)
{
    kerr_no_t ret = kerr_OK;

    if(!paging_is_aligned((virtual_address)))
    {
        ret = -kerr_INVARG;
        goto out;
    }

    /* check the directory and table index */
    *directory_index_out = ((u32)virtual_address / (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_SIZE));
    *table_index_out = ((u32)virtual_address % (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_SIZE) / PAGING_SIZE);

    out:
    return ret;
}

s32 paging_set(u32 *directory, void *virtual_address, u32 value)
{
    kerr_no_t ret = kerr_OK;

    if (!paging_is_aligned(virtual_address))
    {
        ret = -kerr_INVARG;
        goto out;
    }

    u32 directory_index = 0, table_index = 0;

    ret = paging_get_indexes(virtual_address, &directory_index, &table_index);
    if(ret < 0)
    {
        goto out;
    }

    u32  entry = directory[directory_index];

    /**
     * remember that the register 12-31 bit is address
     * */
    u32 *table = (u32 *) (entry & 0xfffff000);

    table[table_index] = value;

    out:
    return ret;
}