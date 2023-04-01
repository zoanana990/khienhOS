#ifndef KHIENHOS_KHEAP_H
#define KHIENHOS_KHEAP_H

#include <khienh/config.h>
#include <khienh/print.h>
#include <memory/heap.h>
#include <memory/memory.h>

void kheap_init();
void *kmalloc(size_t size);
void *kzalloc(size_t size);
void kfree(void *ptr);

#endif // KHIENHOS_KHEAP_H
