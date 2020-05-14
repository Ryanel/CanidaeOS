#ifndef AWD_INFO_H
#define AWD_INFO_H

#include <awd/memmap.h>
#include <stdint.h>

typedef struct {
    // Kernel Info
    uint32_t kernel_phstart;
    uint32_t kernel_phsize;
    // CPU Info
    uint32_t multiboot_struct;
    uint32_t numCPUs;
    // Memory
    uint32_t ptr_phys_mem_map;
    uint32_t ptr_malloc_current;
    uint32_t ptr_malloc_limit;
    // Misc
    uint32_t log_cursor_y;
} awd_info_t;

#endif