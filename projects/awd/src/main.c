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
#include "multiboot.h"

void awd_check_requirements();
void map_kernel(loaded_kernel_info_t indexes);
void enter_kernel(uint32_t startAddress);

awd_info_t awd_boot_info;
uint32_t k_ptr;

loaded_kernel_info_t awd_load(const multiboot_info_t* const mb_info) {
    // Find the kernel
    multiboot_module_t* kernel_module = awd_find_kernel(mb_info);
    console_log("awd", "Bootloader loaded kernel at 0x%08X -> 0x%08X\n", kernel_module->mod_start,
                kernel_module->mod_end);

    // Load the kernel into memory
    loaded_kernel_info_t kernel = awd_load_kernel(kernel_module->mod_start);

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

    // Load kernel
    loaded_kernel_info_t kernel = awd_load(mb_info);

    // Map the kernel
    map_kernel(kernel);

    // Execute the kernel
    console_log("awd", "Entering 64-bit longmode and executing the kernel\n");
    awd_boot_info.log_cursor_y = console_getY();
    k_ptr = kernel.virt_entry;
    enter_kernel(kernel.virt_entry);

    panic("Exited early");
}
