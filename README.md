# CanidaeOS

CandidaeOS is a pure 64-bit OS.

## Projects

* African Wild Dog (awd): A shim bootloader for x86_64, to go from a multiboot to a pure 64-bit environment.
* Lobo: A 64-bit hybrid kernel.

## Building

### Required tools
For building the kernel:
* `binutils` cross compliler
* `gcc` cross compliler
* `make`
* `bash`

For x86_64, generating an iso image also requires:
* `grub`
* `grub-mkrescue`
* `xorriso`

For emulation with the "run" script:
* qemu

For creating documentation, use `doxygen`

### Build instructions

Build options:
* `DIR_BUILD`: Default build/, can be changed via Enviroment variable

To build all projects, use `build.sh`. 
To create and update the documentation, use `builddocs.sh`.

To run the kernel: `run.sh`, and for GDB debugging, `debug.sh` starts QEMU with GDB Stub.