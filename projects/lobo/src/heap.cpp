#include "kernel/heap.h"

#include <stddef.h>
#include <stdint.h>

#include "kernel/log.h"
#include "kernel/panic.h"
#include "kernel/pmm.h"

#include "kernel/mem/liballoc.h"

//#define KERNEL_HEAP_DEBUG_PRINT

enum class AllocMode { Uninitialised, EarlyBoot, Full };

uint64_t  earlyAllocBase  = 0x400000;
uint64_t  earlyAllocLimit = 0x600000;
AllocMode currentMode     = AllocMode::Uninitialised;

uintptr_t heap_early_get_current() { return earlyAllocBase; }
uintptr_t heap_early_get_limit() { return earlyAllocLimit; }

void heap_early_init(uintptr_t base, uintptr_t limit) {
    earlyAllocBase  = base;
    earlyAllocLimit = limit;
    currentMode     = AllocMode::EarlyBoot;
}

void heap_init_full() {
    // Allocate enough starting space...
    // Prime the allocator
    k_extern_liballoc_malloc(0);
    currentMode = AllocMode::Full;
}

void heap_print_debug() {}

void* kmalloc(size_t sz) { return kmalloc_aligned(sz, 8); }
void* kmalloc_page(size_t sz) { return kmalloc_aligned(sz, 0x1000); }

void* kmalloc_aligned(size_t sz, size_t alignment) {
    if (currentMode == AllocMode::Full) {
        // Alright, we need to allocate some space...
        return k_extern_liballoc_malloc(sz);

    } else if (currentMode == AllocMode::EarlyBoot) {
        // During Early Boot, we do not have paging. Do not interact with the VMM, simply allocate space...
        if ((earlyAllocBase % alignment) != 0) { earlyAllocBase += alignment - (earlyAllocBase % alignment); }
        uint64_t block       = earlyAllocBase;
        uint64_t new_current = earlyAllocBase + sz;

        if (new_current > earlyAllocLimit) { panic("kmalloc(early): Ran out of memory!"); }
        earlyAllocBase = new_current;

        #ifdef KERNEL_HEAP_DEBUG_PRINT
        KernelLog::Get().Log("kmalloc", "alloc 0x%x bytes @ 0x%p", sz, block);
        #endif

        return (void*)(block);
    } else {
        panic("Attempted a memory allocation before heap was initialised!");
    }
    return nullptr;
}
