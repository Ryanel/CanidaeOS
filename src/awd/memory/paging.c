#include <awd/paging.h>
#include <awd/alloc.h>
#include <awd/memory.h>
#include <awd/logging.h>
#include <awd_config.h>
#include <awd/kernel.h>
#define PAGING_QUEUE_MAX 10

extern uint64_t paging_pml4[512]; 
unsigned int paging_queue_num = 0;
awd_mapped_region_t paging_queue[PAGING_QUEUE_MAX];

void paging_enable();

void paging_queue_mapping(uint64_t phys_start, uint64_t phys_end, uint64_t virt_start, uint64_t virt_end, uint8_t attribute) {
    paging_queue[paging_queue_num].phys_start = phys_start;
    paging_queue[paging_queue_num].phys_end   = phys_end;
    paging_queue[paging_queue_num].virt_start = virt_start;
    paging_queue[paging_queue_num].virt_end   = virt_end;
    paging_queue[paging_queue_num].attribute  = attribute;
    paging_queue_num++;
}

void paging_queue_print() {
    log_info("paging: Printing queue (n=%d)\n", paging_queue_num);
    for (unsigned int i = 0; i < paging_queue_num && i < PAGING_QUEUE_MAX; i++) {
        log_info("%2d: 0x%016p-0x%016p 0x%016p-0x%016p\n", i, paging_queue[i].phys_start, paging_queue[i].phys_end, paging_queue[i].virt_start, paging_queue[i].virt_end);
    }
}

void paging_init(kernel_loaded_info_t kernel) {
    uint64_t kernel_mem_base = 0xFFFFFFFF80000000;

    paging_queue_mapping(mem.region_low_memory.start, mem.region_low_memory.end, mem.region_low_memory.start, mem.region_low_memory.end, PAGING_ATTRIBUTE_READWRITE);
    paging_queue_mapping(mem.region_awd_image.start, mem.region_awd_image.end, mem.region_awd_image.start, mem.region_awd_image.end, PAGING_ATTRIBUTE_READWRITE);
    paging_queue_mapping(mem.region_awd_malloc.start, mem.region_awd_malloc.end, mem.region_awd_malloc.start, mem.region_awd_malloc.end, PAGING_ATTRIBUTE_READWRITE);

    //paging_queue_mapping(0, 0x200000, 0x0, 0x200000, PAGING_ATTRIBUTE_READWRITE); // TODO, HACK. Fix LOBO!
    paging_queue_mapping(0, 0x200000, 0x0 + kernel_mem_base, 0x200000 + kernel_mem_base, PAGING_ATTRIBUTE_READWRITE); // TODO, HACK. Fix LOBO!

    paging_queue_mapping(kernel.memory_image.start, kernel.memory_image.start + kernel.memory_image.size, kernel.virt.start, kernel.virt.start + kernel.memory_image.size, PAGING_ATTRIBUTE_READWRITE);
    paging_queue_print();
}

void paging_map_page(uint64_t phys, uint64_t virt, uint8_t  page_attr) {
    const uint8_t  attribute       = 0b11;
    const uintptr_t attribute_mask = ~attribute;
    
    uint32_t index_p4 = (virt >> 39) & 511;
    uint32_t index_p3 = (virt >> 30) & 511;
    uint32_t index_p2 = (virt >> 21) & 511;
    uint32_t index_p1 = (virt >> 12) & 511;

    uint64_t *pg_p3, *pg_p2, *pg_p1;

    // Create or Load P3, as needed
    if (paging_pml4[index_p4] == 0) {
        pg_p3                 = (uint64_t *)malloc_aligned(0x1000, 0x1000);
        paging_pml4[index_p4] = (uintptr_t)pg_p3 | attribute;
    } else {
        pg_p3 = (uint64_t *)(((uintptr_t)paging_pml4[index_p4]) & attribute_mask);
    }
    // Create or Load P2, as needed
    if (pg_p3[index_p3] == 0) {
        pg_p2           = (uint64_t *)malloc_aligned(0x1000, 0x1000);
        pg_p3[index_p3] = (uintptr_t)pg_p2 | attribute;
    } else {
        pg_p2 = (uint64_t *)(((uintptr_t)pg_p3[index_p3]) & attribute_mask);
    }
    // Create or Load P1, as needed
    if (pg_p2[index_p2] == 0) {
        pg_p1           = (uint64_t *)malloc_aligned(0x1000, 0x1000);
        pg_p2[index_p2] = (uintptr_t)pg_p1 | attribute;
    } else {
        pg_p1 = (uint64_t *)(((uintptr_t)pg_p2[index_p2]) & attribute_mask);
    }

    uint64_t entry_address = phys;
    uint64_t table_attribute = page_attr;
    uint64_t entry         = entry_address | table_attribute;
    pg_p1[index_p1]        = entry;
    //log_info("page: M PML4[%03d]-> PDP(3)[%03d]-> PD(2)[%03d]-> PT(1)[%03d]=0x%016p\n", index_p4, index_p3, index_p2, index_p1, entry_address);

}

void paging_commit() {
    const uint64_t step = 0x1000;

    for (unsigned int i = 0; i < 512; i++) {paging_pml4[i] = 0;}

    for(unsigned int i = 0; i < paging_queue_num; i++) {
        awd_mapped_region_t region = paging_queue[i];
        uint64_t sz = region.virt_end - region.virt_start;
        
        for (uint64_t j = 0; j < sz; j += step) {
            paging_map_page(region.phys_start + j, region.virt_start + j, region.attribute);
        }
    }
    paging_enable();
    log_info("paging: All pages mapped into memory.\n");
}
