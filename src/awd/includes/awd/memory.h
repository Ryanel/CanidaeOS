#pragma once

#include <stdint.h>
#include <awd_config.h>

#define PAGE_ROUND_UP(x) ( (((uint64_t)(x)) + PAGE_SIZE-1)  & (~(PAGE_SIZE-1)) ) 

struct mem_region {
    uint64_t start;
    uint64_t end;
};

struct mem_info {
    // Physical memory regions
    struct mem_region region_low_memory;      // < 1MB region, hardware reserved
    struct mem_region region_awd_image;       // AWD memory image
    struct mem_region region_multiboot_files; // Multiboot memory region
    struct mem_region region_awd_malloc;      // AWD malloc region
    struct mem_region region_kernel_image;    // Kernel memory image
};


extern struct mem_info mem;


void memory_init();
void memory_heap_init(uint64_t heap_base);
void memory_regions_print();