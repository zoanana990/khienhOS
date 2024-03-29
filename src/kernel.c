#include <khienh/kernel.h>
#include <memory/kheap.h>
#include <memory/page.h>
#include <idt/idt.h>
#include <fs/file.h>
#include <disk/stream.h>

paging_4gb_chunk_t *kernel_chunk = 0;

void kernel_main()
{
    console_init();
    print("khienhOS Copyright @zoanana990 2023 airoha.Inc \n");

    /* initialize the heap */
    kheap_init();

    /* initialize the file system */
    fs_init();

    /* initialize and search the disk */
    disk_search_and_init();

    print_disk_content(disk_get(0));

    /* Initialize the idt */
    idt_init();

    /* Setup paging */
    kernel_chunk = paging_new_4gb(PAGING_WRITE_PRESENT_ACCESS);

    /* switch to kernel paging chunk */
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));

    /* Enable paging */
    enable_paging();

    /* enable interrupt */
    enable_interrupt();

    print_disk_content(disk_get(0));

    s32 fd = fopen("0:/hello.txt", "r");
    print("fd = %d\n", fd);
    if(fd)
    {
        print("fopen successfully\n");
    }

    while(1);
}