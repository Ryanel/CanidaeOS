#ifndef AWD_INFO_H
#define AWD_INFO_H

#include <stdint.h>

#define AWD_MEMMAP_FREE 0
#define AWD_MEMMAP_USED 1

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

typedef struct AWDPhysmemmap {
    uint32_t next;
    char     type;
    uint32_t startAddress_hi;
    uint32_t startAddress_lo;
    uint32_t size_hi;
    uint32_t size_lo;
} awd_physmemmap_t;
#endif