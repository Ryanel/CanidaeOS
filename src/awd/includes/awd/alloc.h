#pragma once

#include <stdint.h>

void alloc_init(uint64_t phys_base);
uint64_t alloc_get_current();
uint64_t alloc_get_limit();
void* malloc(uint32_t size);
void* malloc_aligned(uint32_t size, uint32_t alignment);