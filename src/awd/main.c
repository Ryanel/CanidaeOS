#include <stdio.h>
#include <multiboot.h>

#include <awd/awd.h>
#include <awd/alloc.h>
#include <awd/console.h>
#include <awd/kernel.h>
#include <awd/logging.h>
#include <awd/memory.h>
#include <awd/multiboot.h>
#include <awd/paging.h>
#include <awd/requirements.h>
#include <awd/info.h>

extern uint64_t kernel_entry_point;
extern void kernel_enter();

awd_info_t awd_info;

int awd_main(uint32_t mb_magic, multiboot_info_t* mb_struct) {
    kernel_info_t        kernel_info;
    kernel_loaded_info_t kernel_loaded_info;

    console_init();                                // Initialise the console
    awd_version_display();                         // Display version info
    multiboot_verify(mb_magic);                    // Verify multiboot loaded us correctly.
    awd_cpu_requirements();                        // Check if CPU meets longmode requirements.
    memory_init();                                 // Initialise the memory regions and heap...
    kernel_info = multiboot_analyse(mb_struct);    // Analyse multiboot info
                                                   // TODO: Start any APCPUs
    kernel_info = kernel_analyse(kernel_info);     // Analyse kernel ELF image.
    memory_heap_init(PAGE_ROUND_UP(mem.region_multiboot_files.end));
    kernel_loaded_info = kernel_load(kernel_info); // Create kernel memory image.
    paging_init(kernel_loaded_info);               // Map paging structures from memory image    
    paging_commit();                               // Commit paging structures
    memory_regions_print();
    
    // Setup configuration structures
    awd_info.log_cursor_y = console_get_y();
    awd_info.multiboot_struct = (uint32_t)mb_struct;
    awd_info.ptr_malloc_current = alloc_get_current();
    awd_info.ptr_malloc_limit = alloc_get_limit();
    
    kernel_entry_point = kernel_loaded_info.virtual_entry;
    kernel_enter();                                // Transfer control to the kernel!

    return 0;
}
