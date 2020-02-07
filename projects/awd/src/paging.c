#include "awd/paging.h"

#include <stddef.h>
#include <stdint.h>

#include "awd/console.h"
#include "awd/kernel.h"
#include "memory.h"

awd_mapped_region_t awd_mapped_regions[AWD_MAPPED_REGIONS_MAX];
unsigned int awd_mapped_region_index = 0;
extern uint64_t paging_pml4[512];  // PML4T

extern void paging_enable();

void awd_add_mapped_region_direct(awd_mapped_region_t region) {
    awd_mapped_regions[awd_mapped_region_index] = region;
    awd_mapped_region_index++;
}

void awd_add_mapped_region(uint64_t phys_start, uint64_t phys_end, uint64_t virt_start, uint64_t virt_end) {
    awd_mapped_region_t region;
    region.phys_start = phys_start;
    region.phys_end = phys_end;
    region.virt_start = virt_start;
    region.virt_end = virt_end;
    awd_add_mapped_region_direct(region);
}

void awd_print_mapped_regions() {
    for (unsigned int i = 0; i < awd_mapped_region_index; i++) {
        awd_mapped_region_t region = awd_mapped_regions[i];
        console_log("mapping", "[0x%08X-0x%08X] -> [0x%08X-0x%08X]\n", (uint32_t)region.phys_start,
                    (uint32_t)region.phys_end, (uint32_t)region.virt_start, (uint32_t)region.virt_end);
    }
}

void awd_map_page(uint64_t phys, uint64_t virt) {
    const uint8_t present_writable = 0b11;
    const uintptr_t present_writable_mask = ~present_writable;

    uint32_t index_p4 = (virt >> 39) & 511;
    uint32_t index_p3 = (virt >> 30) & 511;
    uint32_t index_p2 = (virt >> 21) & 511;

    uint64_t *pg_p3, *pg_p2, *pg_p1;

    // Create or Load P3, as needed
    if (paging_pml4[index_p4] == 0) {
        pg_p3 = (uint64_t *)malloc_page(0x1000);
        paging_pml4[index_p4] = (uintptr_t)pg_p3 | present_writable;
    } else {
        pg_p3 = (uint64_t *)(((uintptr_t)paging_pml4[index_p4]) & present_writable_mask);
    }
    // Create or Load P2, as needed
    if (pg_p3[index_p3] == 0) {
        pg_p2 = (uint64_t *)malloc_page(0x1000);
        pg_p3[index_p3] = (uintptr_t)pg_p2 | present_writable;
    } else {
        pg_p2 = (uint64_t *)(((uintptr_t)pg_p3[index_p3]) & present_writable_mask);
    }

    uint64_t entry_address = phys;
    uint64_t attribute = 0b10000011;
    uint64_t entry = entry_address | attribute;
    pg_p2[index_p2] = entry;
    // console_log("paging", "map 4[%03d]-> 3[%03d]-> 2[%03d]\n", index_p4, index_p3, index_p2);
}

void awd_map_pages() {
    // awd_print_mapped_regions();

    // Clear PML4
    for (unsigned int i = 0; i < 512; i++) {
        paging_pml4[i] = 0;
    }

    // Map regions
    for (unsigned int i = 0; i < awd_mapped_region_index; i++) {
        awd_mapped_region_t region = awd_mapped_regions[i];

        // Mapping log
#ifdef DEBUG_LOG
        console_log("mapping", "[0x%08X-0x%08X] -> [", (uint32_t)region.phys_start, (uint32_t)region.phys_end);
        console_print_uint64(region.virt_start);
        console_printstring("->");
        console_print_uint64(region.virt_end);
        console_printstring("]\n");
#endif

        uint64_t k = region.virt_start;
        for (uint64_t j = region.phys_start; j < region.phys_end; j += 0x200000) {
            awd_map_page(j, k);
            k += 0x200000;
        }
    }

    paging_enable();
}
