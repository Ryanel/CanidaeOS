#pragma once

#include <stdint.h>
#include <awd/kernel.h>

typedef struct {
    uint64_t phys_start;
    uint64_t phys_end;
    uint64_t virt_start;
    uint64_t virt_end;
    uint8_t  attribute;
} awd_mapped_region_t;

#define PAGING_ATTRIBUTE_READABLE 0b1
#define PAGING_ATTRIBUTE_READWRITE 0b11

void paging_init(kernel_loaded_info_t kernel);
void paging_queue_mapping(uint64_t phys_start, uint64_t phys_end, uint64_t virt_start, uint64_t virt_end, uint8_t attribute);
void paging_queue_print();
void paging_commit();