#ifndef AWD_KERNEL_H
#define AWD_KERNEL_H

#include <stdint.h>
#include "multiboot.h"

typedef struct LoadedKernelInfo {
    uint64_t phys_entry;
    uint64_t virt_entry;
    uint64_t phys_start;
    uint64_t phys_size;
    uint64_t virt_start;
    uint64_t virt_size;
} loaded_kernel_info_t;

multiboot_module_t *awd_find_kernel(const multiboot_info_t const *mb_info);
loaded_kernel_info_t awd_load_kernel(uint32_t kernelFileAddress);
#endif