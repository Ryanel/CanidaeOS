#ifndef AWD_INFO_H
#define AWD_INFO_H

#include <stdint.h>

#define AWD_MEMMAP_FREE 0
#define AWD_MEMMAP_USED 1

typedef struct {
    uint32_t numCPUs;
    uint32_t kernel_phstart;
    uint32_t kernel_phsize;
    uint32_t multiboot_struct;
    uint32_t log_cursor_y;
    uint32_t ptr_phys_mem_map;
} awd_info_t;

typedef struct AWDPhysmemmap {
    uint32_t next;
    char     type;
    uint32_t startAddress_hi;
    uint32_t startAddress_lo;
    uint64_t size;
} awd_physmemmap_t;
#endif