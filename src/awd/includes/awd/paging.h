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

void paging_commit();
void paging_map_region_direct(awd_mapped_region_t region);
void paging_map_region(uint64_t phys_start, uint64_t phys_end, uint64_t virt_start, uint64_t virt_end);

void paging_print_regions();
#endif