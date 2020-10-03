# African Wild Dog

African Wild Dog (AWD) is a boot-time ELF64 loader.
It will load a 64-bit kernel with paging enabled, and the kernel mapped
correctly.

AWD is loaded by a multiboot compatible bootloader, with the kernel to load
passed as a module.

AWD passes some paramaters to the kernel, including the original multiboot
header.

## Kernel initial state

* All required data is mapped into memory. There is no reference to the page tables, so the page tables must be re-made.
* The kernel configuration structure is filled out with the machine state.
    * This includes the page mappings.

### Kernel responsibilities

The kernel is responsible for re-mapping the memory.

## From Grub Boot Process

This is a summary of the African Wild Dog Boot Process

* Initialise console                           [Done]
* Check multiboot validity.                    [Done]
* Check computer requirements (longmode)       [Done]
* Initialize memory regions                    [Done]
* Initialize malloc.                           [Done]
* Get GRUB Memory MAP                          [?]
* Analyze kernel image                         [Done]
* Load kernel image into memory.               [Done]
* Load modules, if required.                   [?]
* Pre-map all paging regions                   [Done]
* Commit paging structures                     [Done]
* Setup configuration structures
* Enter longmode
* Transfer to kernel.

## Boot process for Other CPUs

* Each AP CPU is booted by the BSP
[TODO]

