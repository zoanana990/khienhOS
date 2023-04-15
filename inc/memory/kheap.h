#ifndef KHIENHOS_KHEAP_H
#define KHIENHOS_KHEAP_H

#include <khienh/config.h>
#include <khienh/print.h>
#include <memory/heap.h>
#include <memory/memory.h>

/**
 * @func: kheap_init
 * @description:
 *      initialize the heap region
 *      set the block size is 4096 bytes, and there are 4096 entries
 *      start address = 0x 1000000
 *      size          = 0x10000000
 *      end address   = 0x11000000
 * */
void kheap_init();


void *kmalloc(size_t size);
void *kzalloc(size_t size);
void kfree(void *ptr);

#endif // KHIENHOS_KHEAP_H
