#include <stdint.h>
#include <stdbool.h>
#include <awd/alloc.h>
#include <awd/memory.h>
#include <awd/logging.h>
#include <awd/panic.h>
#include <awd_config.h>

uint64_t heap_phys_base;
uint64_t heap_phys_current;
uint64_t heap_phys_limit;

bool alloc_enabled = false;

void alloc_init(uint64_t phys_base) {
    heap_phys_base = phys_base;
    heap_phys_current = phys_base;
    heap_phys_limit = phys_base + AWD_ALLOC_SIZE;
    alloc_enabled = true;
}

void alloc_finalize() {
    alloc_enabled = false;
}

uint64_t alloc_get_current() {
    return heap_phys_current;
}

uint64_t alloc_get_limit() {
    return heap_phys_limit;
}

void* malloc(uint32_t size) {
    return malloc_aligned(size, 0);
}

void* malloc_aligned(uint32_t size, uint32_t alignment) {
    if(!alloc_enabled) {
        panic("malloc: Allocation not enabled! (sz: 0x%x, align: 0x%x)\n", size, alignment);
    }

    // Ensure we're aligning to ALLOC_MINIMUM_ALIGNMNET
    alignment = alignment >= AWD_ALLOC_MIN_ALIGNMENT ? alignment : AWD_ALLOC_MIN_ALIGNMENT; 

    // Actually align.
    if ((heap_phys_current % alignment) != 0) { 
        heap_phys_current += alignment - (heap_phys_current % alignment); 
    }

    // Check if we'll hit our limit

    if(heap_phys_current + size >= heap_phys_limit) {
        panic("malloc: Allocation hit limit (sz: 0x%x, align: 0x%x)\n", size, alignment);
    }

    void* allocation = (void*)(heap_phys_current);
    heap_phys_current += size;
    return allocation;
}
