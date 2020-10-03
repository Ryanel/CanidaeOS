#pragma once

#include <stdint.h>

typedef struct kernel_region {
    uint64_t start;
    uint64_t size;
} kernel_region_t;

typedef struct kernel_info {
    uint32_t image_start;
    uint32_t image_end;

    kernel_region_t phys;
    kernel_region_t virt;
} kernel_info_t;

typedef struct kernel_loaded_info {
    kernel_region_t memory_image;
    kernel_region_t virt;
    uint64_t virtual_entry;
} kernel_loaded_info_t;

kernel_info_t kernel_analyse(kernel_info_t info);
kernel_loaded_info_t kernel_load(kernel_info_t info);