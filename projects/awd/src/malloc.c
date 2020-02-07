#include <awd/console.h>
#include <awd/error.h>
#include <stdint.h>

uintptr_t malloc_base = 0x400000;
uintptr_t malloc_current = 0x400000;
uintptr_t malloc_limit = 0x401000;

void init_malloc(uintptr_t base, uintptr_t limit) {
    malloc_current = base;
    malloc_base = base;
    malloc_limit = limit;
}

void* malloc(unsigned int size) {
    const unsigned int alignment = 8;
    // Ensure size is aligned
    if ((size % alignment) != 0) {
        size += alignment - (size % alignment);
    }

    // Ensure current is aligned
    if ((malloc_current % alignment) != 0) {
        malloc_current += alignment - (malloc_current % alignment);
    }

    uintptr_t block = malloc_current;
    uintptr_t new_current = malloc_current + size;

    console_log("malloc", "Allocating 0x%X bytes @ 0x%X\n", size, block);

    if (new_current > malloc_limit) {
        panic("Ran out of memory!");
    }

    malloc_current = new_current;
    return (void*)block;
}

void* malloc_page(unsigned int size) {
    const unsigned int alignment = 0x1000;
    // Ensure size is aligned
    if ((size % alignment) != 0) {
        size += alignment - (size % alignment);
    }

    // Ensure current is aligned
    if ((malloc_current % alignment) != 0) {
        malloc_current += alignment - (malloc_current % alignment);
    }

    uintptr_t block = malloc_current;
    uintptr_t new_current = malloc_current + size;

    // console_log("malloc", "Allocating 0x%X bytes @ 0x%X\n", size, block);

    if (new_current > malloc_limit) {
        panic("Ran out of memory!");
    }

    malloc_current = new_current;
    return (void*)block;
}