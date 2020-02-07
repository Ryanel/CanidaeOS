#ifndef AWD_INFO_H
#define AWD_INFO_H

#include <stdint.h>

typedef enum AWDMemoryRegionType {
    FREE,
    USED_UNKNOWN,
    AWD,
    AWD_HEAP,
    KERNEL,
} awd_memoryregiontype_t;

// Linked list of memory regions with a type
typedef struct awd_info_memory_region {
    enum AWDMemoryRegionType type;
    uint64_t phys_start;
    uint64_t phys_end;
    uint64_t virt_start;
    uint64_t virt_end;
    struct awd_info_memory_region* next;
} awd_info_memory_region_t;

typedef struct {
    uint32_t numCPUs;
    uint32_t kernel_phstart;
    uint32_t kernel_phsize;
    uint32_t multiboot_struct;
    uint32_t log_cursor_y;
    awd_info_memory_region_t* memory_regions;
} awd_info_t;

#endif