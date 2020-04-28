#include <stddef.h>
#include <stdint.h>
#include "stdio.h"

#include "awd/awd_info.h"

#include "kernel/log.h"
#include "kernel/pmm.h"

#include "x86_64/interrupts.h"

#include "drivers/x86_64/uart.h"
#include "drivers/x86_64/vga_console.h"

using namespace Kernel;

void kernel_main();
void kernel_version_print();

UARTLoggingDevice boot_serial_logger_device;
VGAConsoleDevice  boot_vga_console_device;

void init_memory(awd_info_t* awd_info) {
    auto& kernelLog = KernelLog::Get();
    auto& kernelPmm = Kernel::PMM::Get();

    awd_physmemmap_t* awd_memmap = (awd_physmemmap_t*)((uintptr_t)awd_info->ptr_phys_mem_map);

    // Phase 1: Discovery
    uint64_t mem_end_address = 0;
    while (awd_memmap != 0) {
        uint64_t startAddress = ((uint64_t)awd_memmap->startAddress_hi << 32) | (awd_memmap->startAddress_lo);
        uint64_t endAddress   = startAddress + awd_memmap->size;
        if (awd_memmap->type == AWD_MEMMAP_FREE) {
            if (mem_end_address < endAddress) { mem_end_address = endAddress; }
        }
        awd_memmap = (awd_physmemmap_t*)((uintptr_t)awd_memmap->next);
    }

    kernelLog.Log("mem", "Last addressable free RAM is 0x%p, or %p frames", mem_end_address, mem_end_address / 0x1000);
    kernelLog.Log("mem", "Bitmap fits in %p bytes", mem_end_address / 0x1000 / 8 );

    // Phase 2: Freeing unused pages
    kernelPmm.Init(mem_end_address / 0x1000);

    awd_memmap = (awd_physmemmap_t*)((uintptr_t)awd_info->ptr_phys_mem_map);
    while (awd_memmap != 0) {
        uint64_t startAddress = ((uint64_t)awd_memmap->startAddress_hi << 32) | (awd_memmap->startAddress_lo);
        uint64_t endAddress   = startAddress + awd_memmap->size;
        if (awd_memmap->type == AWD_MEMMAP_FREE) {
            for (uint64_t i = startAddress; i < endAddress; i += 0x1000) { kernelPmm.FreePage(i); }
        }
        awd_memmap = (awd_physmemmap_t*)((uintptr_t)awd_memmap->next);
    }
}

extern "C" void kernel_entry(awd_info_t* awd_info) {
    awd_info        = awd_info;
    auto& kernelLog = KernelLog::Get();

    // Architecture specific setup
    // Start VGA and Logging
    boot_vga_console_device.Init(0, awd_info->log_cursor_y);
    kernelLog.SetSerialLogging(&boot_serial_logger_device);
    kernelLog.SetTerminalDevice(&boot_vga_console_device);

    // Print the kernel version
    kernel_version_print();

    // Interrupts
    kernelLog.Log("int", "Initialising interrupts");
    init_idt();
    kernelLog.Log("int", "Interrupts enabled");

    // Memory
    init_memory(awd_info);

    // Architecture specific setup finished, boot into kernel main...
    kernel_main();
}