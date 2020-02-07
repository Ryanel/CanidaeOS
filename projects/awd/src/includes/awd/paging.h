#ifndef AWD_PAGING_H
#define AWD_PAGING_H

#include <stdint.h>

#define AWD_MAPPED_REGIONS_MAX 10

typedef struct {
    uint64_t phys_start;
    uint64_t phys_end;
    uint64_t virt_start;
    uint64_t virt_end;
} awd_mapped_region_t;

void awd_map_pages();
void awd_add_mapped_region_direct(awd_mapped_region_t region);
void awd_add_mapped_region(uint64_t phys_start, uint64_t phys_end, uint64_t virt_start, uint64_t virt_end);

void awd_print_mapped_regions();
#endif