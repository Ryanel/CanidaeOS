# Loading the kernel into memory in AWD

The kernel is loaded as an ELF file into memory by the multiboot bootloader.
African Wild Dog's job is to parse the kernel and load it to the correct memory areas.

## Parsing & Loading

1. Initially, AWD will check to make sure that the loaded module by GRUB is infact a x86_64 ELF file.
2. Then, it gathers information like the virtual base address and memory size.
3. After that, it will go over each LOAD Program Header, and load the section from the memory image into a buffer

## Paging
AWD takes the required sections of the kernel and maps them to where they're expected to be.

Specifically:

* The two megabytes are identity mapped
* The kernel is mapped to its proper virtual address.
* Modules are unmapped (unless they are within the first two megabytes)

## Configuration

The current state of the hardware and multiboot header are saved in a awd_info_t structure, which is passed by AWD into the kernel.