#include <stdbool.h>
#include <string.h>
#include <elf64.h>

#include <awd/alloc.h>
#include <awd/kernel.h>
#include <awd/logging.h>
#include <awd/memory.h>
#include <awd/panic.h>

bool elf_check(elf64_header_t *elf_header) {
    if(elf_header->e_ident[0] != 0x7F) { return false; }
    if(elf_header->e_ident[1] != 'E')  { return false; }
    if(elf_header->e_ident[2] != 'L')  { return false; }
    if(elf_header->e_ident[3] != 'F')  { return false; }

    // Check if machine type is AMD64 and type is executable.
    if(elf_header->e_machine != 0x3E)  { return false; }
    if(elf_header->e_type != 2)        { return false; }

    return true;
}

kernel_info_t kernel_analyse(kernel_info_t info) {
    elf64_header_t * elf_header = (elf64_header_t*)info.image_start;

    if(elf_check(elf_header) == false) {
        panic("kernel error: Kernel is not a ELF executable.");
    }

    kernel_region_t phys;
    kernel_region_t virt;

    virt.start = 0xFFFFFFFFFFFFFF00;
    phys.start = 0xFFFFFFFFFFFFFF00;
    phys.size  = 0;
    virt.size  = 0;

    // Go through each program header.
    for (unsigned int i = 0; i < elf_header->e_phnum; i++) {
        // Get a pointer to program header
        uint32_t offset = elf_header->e_phoff + (elf_header->e_phentsize * i);
        elf64_pheader_t * prog_header = (elf64_pheader_t *)(info.image_start + offset);

        // Ignore non-LOAD headers.
        if (prog_header->p_type != 1) { continue; } 

        // Set the virtual start to the lowest LOAD vaddr
        if (virt.start > prog_header->p_vaddr) { virt.start = prog_header->p_vaddr; }
        
        // Check if the physical address 
        uint32_t ph_file_address = info.image_start + prog_header->p_offset;
        if (phys.start > ph_file_address) { phys.start = ph_file_address; }
        
        // Calculate the physical size.
        phys.size += (uint32_t)prog_header->p_memsz;
        if ((phys.size % 0x1000) != 0) {
            phys.size += 0x1000 - ((uint32_t)phys.size % 0x1000);
        }
    }
    
    // Size in physical memory is size in virtual.
    virt.size = phys.size;
    info.phys = phys;
    info.virt = virt;

    log_info("kernel: Kernel phys. memory image is @ 0x%016p with size %x\n", phys.start, phys.size);
    log_info("kernel: Kernel virt. memory image is @ 0x%016p with size %x\n", virt.start, virt.size);
    
    return info;
}

kernel_loaded_info_t kernel_load(kernel_info_t info) {
    kernel_loaded_info_t loaded_kernel_info;
    elf64_header_t * elf_header = (elf64_header_t*)info.image_start;
    uintptr_t kernel_phys_image = (uintptr_t)malloc_aligned(info.phys.size, 0x1000);

    // Load headers
    for (unsigned int i = 0; i < elf_header->e_phnum; i++) {
        // Get a pointer to current program header
        uint32_t offset = elf_header->e_phoff + (elf_header->e_phentsize * i);
        elf64_pheader_t * prog_header = (elf64_pheader_t *)(info.image_start + offset);

        // Ignore non-LOAD headers.
        if (prog_header->p_type != 1) { continue; } 

        // Properties 
        uint32_t ph_file_address = info.image_start + prog_header->p_offset;
        uint32_t dest_address    = kernel_phys_image + ((prog_header->p_vaddr) - info.virt.start);

        // Copy from ELF image to memory image!
        memcpy((void *)dest_address, (void *)ph_file_address, prog_header->p_filesz);
    }

    loaded_kernel_info.memory_image.start = kernel_phys_image;
    loaded_kernel_info.memory_image.size  = info.phys.size;
    loaded_kernel_info.virt               = info.virt;
    loaded_kernel_info.virtual_entry      = elf_header->e_entry;

    // Set memory region...
    mem.region_kernel_image.start         = kernel_phys_image;
    mem.region_kernel_image.end           = kernel_phys_image + info.phys.size - 1;
    return loaded_kernel_info;
}