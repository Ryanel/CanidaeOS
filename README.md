# CanidaeOS

CandidaeOS is a pure 64-bit OS.

## Projects

* African Wild Dog (awd): A shim bootloader for x86_64, to go from a multiboot to a pure 64-bit environment.
* Lobo: A 64-bit microkernel.

## Building

### Required tools
For building the kernel:
* binutils cross compliler
* gcc cross compliler
* make
* bash

For x86_64, generating an iso image also requires:
* grub
* grub-mkrescue
* xorriso

For emulation with the "run" script:
* qemu