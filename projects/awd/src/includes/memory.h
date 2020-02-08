#ifndef AWD_MEMORY_H
#define AWD_MEMORY_H
#include <stdint.h>

void init_malloc(uintptr_t base, uintptr_t limit);
void* malloc_aligned(unsigned int size, unsigned int alignment);
void* malloc(unsigned int size);
void* malloc_page(unsigned int size);
#endif