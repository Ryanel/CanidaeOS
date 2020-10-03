#include <awd/alloc.h>
#include <awd/memory.h>
#include <awd/logging.h>
#include <awd_config.h>
struct mem_info mem;

// Linker symbols
extern uint32_t awd_phys_start;
extern uint32_t awd_phys_end;

void region_print(const char * name, struct mem_region region) {
    log_info(" * %19s : %016p -> %016p\n", name, region.start, region.end);   
}

void memory_regions_print() {
    log_info("mem: Printing current memory regions\n");
    
    log_info("   %19s : %-16s    %-16s\n", "Region Name", "Start", "End");   

    region_print("Low Memory",   mem.region_low_memory);
    region_print("AWD Memory Image",    mem.region_awd_image);
    region_print("Multiboot Modules", mem.region_multiboot_files);
    region_print("AWD Alloc",    mem.region_awd_malloc);
    region_print("Kernel Memory Image", mem.region_kernel_image);

    uint64_t kb_taken = (mem.region_awd_malloc.end - mem.region_awd_image.start) / 1024;
    log_info(" * Memory regions are tracking %d kb of high memory.\n", kb_taken);
}

void memory_init() {
    // Low memory
    mem.region_low_memory.start = 0x0;      // Start of memory
    mem.region_low_memory.end   = 0x100000 - 1; // 1MB

    // AWD Memory image 
    mem.region_awd_image.start  = (uint64_t)&awd_phys_start; 
    mem.region_awd_image.end    = PAGE_ROUND_UP(((uint64_t)&awd_phys_end)) - 1; 
    
}

void memory_heap_init(uint64_t heap_base) {
    mem.region_awd_malloc.start = heap_base;
    mem.region_awd_malloc.end   = heap_base + AWD_ALLOC_SIZE - 1;
    alloc_init(heap_base);
}