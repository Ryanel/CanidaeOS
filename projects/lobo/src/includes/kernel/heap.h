#pragma once

#include <stddef.h>
#include <stdint.h>

void heap_early_init(uintptr_t base, uintptr_t limit);
void heap_init_full();

uintptr_t heap_early_get_current();
uintptr_t heap_early_get_limit();

void* kmalloc(size_t sz);
void* kmalloc_aligned(size_t sz, size_t alignment);
void* kmalloc_page(size_t sz);