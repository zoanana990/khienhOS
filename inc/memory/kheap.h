#ifndef KHIENHOS_KHEAP_H
#define KHIENHOS_KHEAP_H

#include <khienh/config.h>
#include <khienh/print.h>
#include <memory/heap.h>

void kheap_init();
void *kmalloc(size_t size);

#endif // KHIENHOS_KHEAP_H
