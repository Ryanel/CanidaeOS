#ifndef AWD_INFO_H
#define AWD_INFO_H

#include <stdint.h>

typedef struct {
    uint32_t numCPUs;
    uint32_t kernel_phstart;
    uint32_t kernel_phsize;
    uint32_t multiboot_struct;
    uint32_t log_cursor_y;
    uint32_t phys_mem_map;
} awd_info_t;

#endif