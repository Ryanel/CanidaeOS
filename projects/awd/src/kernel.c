#include "awd/kernel.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "awd/console.h"
#include "awd/error.h"
#include "elf64.h"
#include "multiboot.h"
#include "stdio.h"

multiboot_module_t *awd_find_kernel(const multiboot_info_t const *mb_info) {
    if (mb_info->mods_count == 0) {
        panic(
            "No kernel found, no modules were loaded by the multiboot "
            "bootloader.");
    }

    uintptr_t module_struct_address = (uintptr_t)(mb_info->mods_addr);
    multiboot_module_t *mod = (multiboot_module_t *)module_struct_address;
    return mod;
}

int elf_check_executable(elf64_header_t *elf_header) {
    if (elf_header->e_ident[0] != 0x7f || elf_header->e_ident[1] != 'E' || elf_header->e_ident[2] != 'L' ||
        elf_header->e_ident[3] != 'F') {
        return 0;
    }
    if (elf_header->e_machine != 0x3E) {
        return 0;
    }
    if (elf_header->e_type != 2) {
        return 0;
    }
    return 1;
}

loaded_kernel_info_t awd_load_kernel(uint32_t baseAddress) {
    loaded_kernel_info_t kernel_info;
    elf64_header_t *elf_header = (elf64_header_t *)baseAddress;

    if (elf_check_executable(elf_header) == 0) {
        panic("Kernel is not a valid loadable ELF64 executable");
    }
    console_log("elf64", "- ELF Info ----------------------------------------------------------\n");
    console_log("elf64", "%19s = 0x%08X\n", "mod_base", baseAddress);
    console_log("elf64", "%19s = 0x%08X\n", "entry_point", elf_header->e_entry);
    console_log("elf64", "%19s = %d\n", "num_program_headers", elf_header->e_phnum);
    console_log("elf64", "%19s = %d\n", "num_section_headers", elf_header->e_shnum);
    console_log("elf64", "- Load Log ----------------------------------------------------------\n");

    // console_log("elf64", "LOAD (PH%d) from 0x%08X (%d bytes) -> 0x%08X\n", 0, 100, 100, 1000);

    uint32_t kernel_phys_address = 0x400000;

    kernel_info.virt_start = 0xFFFFFFFFFFFFFFF;
    kernel_info.phys_start = 0xFFFFFFFFFFFFFFF0;

    for (unsigned int i = 0; i < elf_header->e_phnum; i++) {
        uint32_t ph_offset = elf_header->e_phoff + (elf_header->e_phentsize * i);
        elf64_pheader_t *pheader = (elf64_pheader_t *)(baseAddress + ph_offset);

        // We don't care about anything that isn't a load program header, so skip it.
        if (pheader->p_type != 1) {
            continue;
        }
        uint32_t size = pheader->p_filesz;
        uint32_t ph_file_address = baseAddress + pheader->p_offset;
        uint32_t dest_offset = (pheader->p_vaddr - kernel_phys_address);
        uint32_t dest_address = kernel_phys_address + dest_offset;

        if (kernel_info.virt_start > pheader->p_vaddr) {
            kernel_info.virt_start = pheader->p_vaddr;
        }
        if (kernel_info.phys_start > ph_file_address) {
            kernel_info.phys_start = ph_file_address;
        }

        memcpy((void *)dest_address, (void *)ph_file_address, size);

        uint32_t vaddr_lo = pheader->p_vaddr & 0xFFFFFFFF;
        uint32_t vaddr_hi = pheader->p_vaddr >> 32;

        console_log("elf64", "LOAD<%d> from [p]0x%08X -> [p]0x%08X [v]0x%08X%08X\n", i, ph_file_address, dest_address,
                    vaddr_hi, vaddr_lo);
    }

    console_log("elf64", "---------------------------------------------------------------------\n");

    kernel_info.virt_size = 0x40000;
    kernel_info.phys_size = 0x40000;

    kernel_info.virt_entry = elf_header->e_entry;
    kernel_info.phys_start = kernel_phys_address;
    return kernel_info;
}
