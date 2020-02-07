# Loading the kernel into memory in AWD

The kernel is loaded as an ELF file into memory by the multiboot bootloader.
African Wild Dog's job is to parse the kernel and load it to the correct memory areas.

## Parsing
Initially, AWD will check to make sure that the loaded module by GRUB is infact a 64-bit x86 ELF file. It will then take note and save any needed information for later.

Then, it will go over each LOAD program buffer, and load the section into a buffer.

## Paging
AWD will take the required sections of the kernel and map them to where they're expected to be.

Specifically:

* The first megabyte is identity mapped
* The kernel is mapped to it's proper virtual address, while physically it's still in the buffer.
* Modules are unmapped.