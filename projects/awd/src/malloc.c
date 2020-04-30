#include <awd/console.h>
#include <awd/error.h>
#include <stdint.h>

#include "memory.h"

//#define DEBUG_LOG_MALLOC

uintptr_t malloc_base    = 0x200000;
uintptr_t malloc_current = 0x200000;
uintptr_t malloc_limit   = 0x400000;

uintptr_t malloc_get_current() {
    return malloc_current;
}

uintptr_t malloc_get_limit() {
    return malloc_limit;
}

void init_malloc(uintptr_t base, uintptr_t limit) {
    malloc_current = base;
    malloc_base    = base;
    malloc_limit   = limit;

#ifdef DEBUG_LOG_MALLOC
    console_log("malloc", "Initalised @ 0x%X\n", malloc_current);
#endif
}

void* malloc_aligned(unsigned int size, unsigned int alignment) {
    // Ensure next address is aligned
    if ((malloc_current % alignment) != 0) { malloc_current += alignment - (malloc_current % alignment); }
    uintptr_t block       = malloc_current;
    uintptr_t new_current = malloc_current + size;
#ifdef DEBUG_LOG_MALLOC
    console_log("malloc", "Allocating 0x%X bytes @ 0x%X\n", size, block);
#endif
    if (new_current > malloc_limit) { panic("Ran out of memory!"); }

    malloc_current = new_current;
    return (void*)block;
}

void* malloc(unsigned int size) { return malloc_aligned(size, 8); }
void* malloc_page(unsigned int size) { return malloc_aligned(size, 0x1000); }