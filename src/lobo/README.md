# Lobo Kernel

Lobo is a pure 64-bit hybrid kernel. Most services will run in Userspace.

## Requirements
* 64-bit CPU
* 8MB of memory.
* x86: APIC. 

## Features
* Boots from AWD in 64-bit mode.
* VGA Output
* Serial Output
* Logging Facilities
* x86_64: GDT
* x86_64: IDT
* Physical Memory Manager
* Virtual Memory Manager (in progress)

### To-do
* Module Loader
* Message Passing System
* PCI
* Timing Subsystem
* Processes
* Process Scheduling
* Multiprocessor Support
* VFS
