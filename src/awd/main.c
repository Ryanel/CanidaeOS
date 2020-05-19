//-----------------------------------------------------------------------------
// main.c
// (C) 2020 Corwin McKnight
// Purpose: Find & Load the ELF64 Kernel, then boot it.
//-----------------------------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>

#include "awd/console.h"
#include "awd/error.h"
#include "awd/info.h"
#include "awd/kernel.h"
#include "awd/memmap.h"
#include "awd/paging.h"
#include "memory.h"
#include "multiboot.h"

void awd_check_requirements();
void enter_kernel(uint32_t startAddress);

awd_info_t awd_boot_info;
uint64_t   k_ptr;

extern uint32_t          awd_phys_start;
extern uint32_t          awd_phys_end;
extern multiboot_info_t* multiboot_struct_ptr;

loaded_kernel_info_t awd_load() {
    // Find the kernel
    multiboot_module_t* kernel_module = awd_find_kernel(multiboot_struct_ptr);
    console_log("awd", "Bootloader loaded kernel at 0x%08X -> 0x%08X\n", kernel_module->mod_start,
                kernel_module->mod_end);

    // Load the kernel into memory
    loaded_kernel_info_t kernel = awd_load_kernel(kernel_module->mod_start);
    awd_mapped_region_t  kernel_mapped_region;
    kernel_mapped_region.phys_start = kernel.phys_start;
    kernel_mapped_region.phys_end   = kernel.phys_start + kernel.phys_size;
    kernel_mapped_region.virt_start = kernel.virt_start;
    kernel_mapped_region.virt_end   = kernel.virt_start + kernel.virt_size;
    paging_map_region_direct(kernel_mapped_region);

    return kernel;
}

void find_memory() {
    uint32_t mmap_length = multiboot_struct_ptr->mmap_length;

    for (uint32_t mmap_base = multiboot_struct_ptr->mmap_addr;
         mmap_base < multiboot_struct_ptr->mmap_addr + mmap_length;) {
        multiboot_memory_map_t* mapping = (multiboot_memory_map_t*)mmap_base;

        mmap_base += 24;

        uint64_t addr     = mapping->addr;
        uint64_t end_addr = mapping->addr + mapping->len;

        const char* type_string;
        char        type = AWD_MEMMAP_FREE;

        switch (mapping->type) {
            case MULTIBOOT_MEMORY_AVAILABLE:
                type_string = "Free";
                type        = AWD_MEMMAP_FREE;
                break;
            default:
                type_string = "Claimed";
                type        = AWD_MEMMAP_USED;
                break;
        }

        console_log("mmap", "[ %7s ] ", type_string);
        console_print_uint64(addr);
        console_printstring(" -> ");
        console_print_uint64(end_addr);
        console_printchar('\n');

        memmap_phys_add(memmap_phys_create(type, (uint32_t)(addr & 0xFFFFFFFF), ((addr & 0xFFFFFFFF00000000) >> 32),
                                           (mapping->len & 0xFFFFFFFF),
                                           ((mapping->len & 0xFFFFFFFF00000000) >> 32)
                                        ));
    }
}

void awd_main(uint32_t mb_magic) {
    // Initialise the console
    console_init();
    console_log("awd", "African Wild Dog Loader [v 0.0.0.2]\n");

    // Check to make sure we booted successfully
    if (mb_magic != 0x2BADB002) { panic("Multiboot magic was incorrect"); }

    // Check the requirements
    awd_check_requirements();

    // Find the malloc region (another 2MB)
    init_malloc(0x200000, 0x400000);

    // Map the next 2MB, for use by the kernel's initial page mapping code.
    paging_map_region(0x200000, 0x400000, 0x200000, 0x400000);
    paging_map_region(0x200000, 0x400000, (0xffffffff80000000 + 0x200000), (0xffffffff80000000 + 0x400000));

    // Load kernel
    loaded_kernel_info_t kernel = awd_load();

    // Identity map the first 2 MB, then commit
    paging_map_region(0, 0x200000, 0, 0x200000);
    paging_map_region(0, 0x200000, (0xffffffff80000000 + 0x000000), (0xffffffff80000000 + 0x200000));

    paging_commit();

    // Parse memory map...
    console_log("mmap", "Parsing memory map from GRUB\n");
    find_memory();

    // Execute the kernel
    console_log("awd", "Entering 64-bit longmode and executing the kernel\n");

    // Write configuration
    k_ptr                            = kernel.virt_entry;
    awd_boot_info.numCPUs            = 1;
    awd_boot_info.ptr_phys_mem_map   = (uint32_t)memmap_phys_get_root();
    awd_boot_info.multiboot_struct   = (uint32_t)multiboot_struct_ptr;
    awd_boot_info.kernel_phstart     = kernel.phys_start;
    awd_boot_info.kernel_phsize      = kernel.phys_size;
    awd_boot_info.log_cursor_y       = console_getY();
    awd_boot_info.ptr_malloc_current = malloc_get_current();
    awd_boot_info.ptr_malloc_limit   = malloc_get_limit();
    enter_kernel(kernel.virt_entry);
    panic("Exited early");
}
