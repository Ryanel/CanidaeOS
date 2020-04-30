#include "kernel/heap.h"

#include <stddef.h>
#include <stdint.h>

#include "kernel/log.h"
#include "kernel/panic.h"

enum class AllocMode { Uninitialised, EarlyBoot, Full };

AllocMode currentMode = AllocMode::Uninitialised;

uint64_t earlyAllocBase  = 0x400000;
uint64_t earlyAllocLimit = 0x600000;

uintptr_t heap_early_get_current() { return earlyAllocBase; }
uintptr_t heap_early_get_limit() { return earlyAllocLimit; }

void heap_early_init(uintptr_t base, uintptr_t limit) {
    earlyAllocBase  = base;
    earlyAllocLimit = limit;
    currentMode     = AllocMode::EarlyBoot;
}

void heap_init_full() { currentMode = AllocMode::Full; }

void* kmalloc_aligned(size_t sz, size_t alignment) {
    if (currentMode == AllocMode::EarlyBoot) {
        // During Early Boot, we do not have paging. Do not interact with the VMM, simply allocate space...
        if ((earlyAllocBase % alignment) != 0) { earlyAllocBase += alignment - (earlyAllocBase % alignment); }
        uint64_t block       = earlyAllocBase;
        uint64_t new_current = earlyAllocBase + sz;

        if (new_current > earlyAllocLimit) { panic("kmalloc(early): Ran out of memory!"); }
        KernelLog::Get().Log("kmalloc", "alloc 0x%x bytes @ 0x%p", sz, block);
        earlyAllocBase = new_current;
        return (void*)(block);
    } else {
        panic("Attempted a memory allocation before heap was initialised!");
    }
    return nullptr;
}

void* kmalloc(size_t sz) { return kmalloc_aligned(sz, 8); }
void* kmalloc_page(size_t sz) { return kmalloc_aligned(sz, 0x1000); }