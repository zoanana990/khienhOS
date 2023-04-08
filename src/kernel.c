#include <khienh/kernel.h>
#include <khienh/type.h>
#include <khienh/font_color.h>
#include <memory/kheap.h>
#include <memory/page.h>
#include <idt/idt.h>
#include <common/string.h>
#include <fs/file.h>
#include <disk/stream.h>

paging_4gb_chunk_t *kernel_chunk = 0;

void kernel_main()
{
    console_init();
    print("Hello World\n");

    /* Initialize the idt */
    idt_init();

    /* initialize the heap */
    kheap_init();

    /* initialize the file system */
    fs_init();

    /* Setup paging */
    kernel_chunk = paging_new_4gb(PAGING_WRITE_PRESENT_ACCESS);

    /* switch to kernel paging chunk */
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));

    /* Enable paging */
    enable_paging();

    /* enable interrupt */
    enable_interrupt();

    ds_t* stream = disk_streamer_new(0);
    disk_streamer_seek(stream, 0x201);
    unsigned char c = 0;
    disk_streamer_read(stream, &c, 1);
    while(1) {}
}