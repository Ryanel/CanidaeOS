# African Wild Dog

African Wild Dog (AWD) is a boot-time ELF64 loader.
It will load a 64-bit kernel with paging enabled, and the kernel mapped
correctly.

AWD is loaded by a multiboot compatible bootloader, with the kernel to load
passed as a module.

AWD passes some paramaters to the kernel, including the original multiboot
header.
