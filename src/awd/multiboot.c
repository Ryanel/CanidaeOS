#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <multiboot.h>
#include <awd/kernel.h>
#include <awd/logging.h>
#include <awd/memory.h>
#include <awd/multiboot.h>
#include <awd/panic.h>
#include <awd_config.h>

void multiboot_verify(uint32_t magic) {
    if(magic != 0x2BADB002) {       
        panic("multiboot: Multiboot loading failed (magic is incorrect). Halting.");
    }
}

multiboot_module_t * multiboot_find_kernel(multiboot_info_t* mb_struct) {
    uintptr_t module_struct_address = (uintptr_t)(mb_struct->mods_addr);
    uintptr_t module_struct_num     = (uintptr_t)(mb_struct->mods_count);

    // Search each module for "kernel" in the command line
    for (size_t i = 0; i < module_struct_num; i++) {
        multiboot_module_t *mod = (multiboot_module_t *)(module_struct_address + (sizeof(multiboot_module_t) * i));
        if (strcmp((const char *)mod->cmdline, KERNEL_TAG_NAME) == 0) { return mod; }
    }

    if (mb_struct->mods_count == 0) {
        panic(
            "multiboot: No kernel found, no modules were loaded by the multiboot bootloader.");
    }

    return 0;
}

kernel_info_t multiboot_analyse(multiboot_info_t* mb_struct) {
    // Locate kernel, get memory map, get graphics.
    log_info("multiboot: Bootloader is %s\n", mb_struct->boot_loader_name);

    // Find the kernel
    multiboot_module_t * kernel_module = multiboot_find_kernel(mb_struct);

    kernel_info_t kernel_info;
    kernel_info.image_start = kernel_module->mod_start;
    kernel_info.image_end   = kernel_module->mod_end;
    
    // Update memory region
    mem.region_multiboot_files.start = PAGE_ROUND_UP(kernel_info.image_start);
    mem.region_multiboot_files.end = PAGE_ROUND_UP(kernel_info.image_end) - 1;

    return kernel_info;
}