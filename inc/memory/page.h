#ifndef KHIENHOS_PAGE_H
#define KHIENHOS_PAGE_H

#include <khienh/type.h>

/*** Paging table entry
 * Reference: https://wiki.osdev.org/Paging
 * [bit 12 - 31]: address
 * [bit 09 - 11]: AVL, available bit
 * [bit 8]: G, Global
 * [bit 7]: PAT, Page attribute table
 * [bit 6]: D, Dirty
 * [bit 5]: A, Accessed
 * [bit 4]: PCD, Cache disable
 * [bit 3]: PWT, Write-through or write-back
 * [bit 2]: U/S, User/Supervisor
 * [bit 1]: R/W, Read or write
 * [bit 0]: P, Present
 ***/
#define PAGING_CACHE_DISABLED           0b00010000
#define PAGING_WRITE_THROUGH            0b00001000
#define PAGING_ACCESS_FROM_ALL          0b00000100
#define PAGING_IS_WRITABLE              0b00000010
#define PAGING_IS_PRESENT               0b00000001

#define PAGING_TOTAL_ENTRIES_PER_TABLE  1024
#define PAGING_SIZE                     4096

typedef struct paging_4gb_chunk
{
    u32 *directory_entry;
}paging_4gb_chunk_t;

paging_4gb_chunk_t* paging_new_4gb(u8 flags);
void paging_switch(u32* directory);
void enable_paging();
u32* paging_4gb_chunk_get_directory(paging_4gb_chunk_t* chunk);

#endif //KHIENHOS_PAGE_H
