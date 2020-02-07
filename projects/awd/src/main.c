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
#include "awd/paging.h"
#include "memory.h"
#include "multiboot.h"

void awd_check_requirements();
void enter_kernel(uint32_t startAddress);

awd_info_t awd_boot_info;
uint64_t k_ptr;

loaded_kernel_info_t awd_load(const multiboot_info_t* const mb_info) {
    // Find the kernel
    multiboot_module_t* kernel_module = awd_find_kernel(mb_info);
    console_log("awd", "Bootloader loaded kernel at 0x%08X -> 0x%08X\n", kernel_module->mod_start,
                kernel_module->mod_end);

    // Load the kernel into memory
    loaded_kernel_info_t kernel = awd_load_kernel(kernel_module->mod_start);
    awd_mapped_region_t kernel_mapped_region;
    kernel_mapped_region.phys_start = kernel.phys_start;
    kernel_mapped_region.phys_end = kernel.phys_start + kernel.phys_size;
    kernel_mapped_region.virt_start = kernel.virt_start;
    kernel_mapped_region.virt_end = kernel.virt_start + kernel.virt_size;
    awd_add_mapped_region_direct(kernel_mapped_region);
    return kernel;
}

void awd_main(uint32_t mb_magic, const multiboot_info_t* const mb_info) {
    // Initialise the console
    console_init();

    console_log("awd", "African Wild Dog Loader [v 0.0.0.1]\n");

    // Check to make sure we booted successfully
    if (mb_magic != 0x2BADB002) {
        panic("Multiboot magic was incorrect");
    }

    // Check the requirements
    awd_check_requirements();

    // Identity map the first 2 MB
    awd_add_mapped_region(0, 0x200000, 0, 0x200000);

    // Then map the malloc region (another 2MB)
    // HACK: No idea if anything gets clobbered due to this!
    // TODO: Can conserve memory by starting malloc at end of grub modules
    awd_add_mapped_region(0x200000, 0x400000, 0x200000, 0x400000);
    init_malloc(0x200000, 0x400000);

    // Load kernel
    loaded_kernel_info_t kernel = awd_load(mb_info);

    // Map everything
    awd_map_pages();

    // Execute the kernel
    console_log("awd", "Entering 64-bit longmode and executing the kernel\n");

    awd_boot_info.log_cursor_y = console_getY();
    k_ptr = kernel.virt_entry;

    enter_kernel(kernel.virt_entry);

    panic("Exited early");
}
