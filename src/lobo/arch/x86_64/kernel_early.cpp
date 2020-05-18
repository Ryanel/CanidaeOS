#include <stddef.h>
#include <stdint.h>
#include "stdio.h"

#include "awd/awd_info.h"

#include "kernel/heap.h"
#include "kernel/kernel.h"
#include "kernel/log.h"
#include "kernel/pmm.h"
#include "kernel/vmm.h"

#include "kernel/arch/x86_64/interrupts.h"

#include "kernel/arch/x86_64/drivers/uart.h"
#include "kernel/arch/x86_64/drivers/vga_console.h"

using namespace kernel;

void kernel_main();
void kernel_version_print();

UARTLoggingDevice boot_serial_logger_device;
VGAConsoleDevice  boot_vga_console_device;

void init_memory(awd_info_t* awd_info) {
    auto& kernelLog = KernelLog::Get();
    auto& kernelPmm = PMM::Get();

    awd_physmemmap_t* awd_memmap = (awd_physmemmap_t*)(MEM_PHYS_TO_VIRT(awd_info->ptr_phys_mem_map));

    // Phase 1: Discovery
    physical_addr_t mem_end_address = 0;
    while ((virtual_addr_t)awd_memmap != MEM_PHYS_TO_VIRT(0)) {
        physical_addr_t startAddress = (((uint64_t)(awd_memmap->startAddress_hi)) << 32) | awd_memmap->startAddress_lo;
        physical_addr_t size         = ((uint64_t)(awd_memmap->size_hi) << 32) | awd_memmap->size_lo;
        physical_addr_t endAddress   = startAddress + size;

        if (awd_memmap->type == AWD_MEMMAP_FREE) {
            if (mem_end_address < endAddress) { mem_end_address = endAddress; }
        }
        awd_memmap = (awd_physmemmap_t*)(MEM_PHYS_TO_VIRT(awd_memmap->next));
    }

    kernelLog.Log("mem", "Last addressable free RAM is 0x%p, or %p frames", mem_end_address, mem_end_address / 0x1000);
    kernelLog.Log("mem", "Bitmap fits in %p bytes", mem_end_address / 0x1000 / 8);

    // Phase 2: Initialse the PMM
    kernelPmm.Init(mem_end_address / 0x1000);

    awd_memmap = (awd_physmemmap_t*)(MEM_PHYS_TO_VIRT(awd_info->ptr_phys_mem_map));
    while ((virtual_addr_t)awd_memmap != MEM_PHYS_TO_VIRT(0)) {
        physical_addr_t startAddress = (((uint64_t)(awd_memmap->startAddress_hi)) << 32) | awd_memmap->startAddress_lo;
        physical_addr_t size         = ((uint64_t)(awd_memmap->size_hi) << 32) | awd_memmap->size_lo;
        physical_addr_t endAddress   = startAddress + size;

        if (awd_memmap->type == AWD_MEMMAP_FREE) {
            for (physical_addr_t i = startAddress; i < endAddress; i += 0x1000) { kernelPmm.FreePage(i); }
        }
        awd_memmap = (awd_physmemmap_t*)(MEM_PHYS_TO_VIRT(awd_memmap->next));
    }

    // Forcibly mark the region below 4MB as used
    // HACK: Fix with proper memory region marking...
    for (uint64_t i = 0; i < 0x400000; i += 0x1000) { kernelPmm.SetPage(i); }

    kernelPmm.SetFreeMemory();
}

extern "C" void kernel_early(awd_info_t* awd_info) {
    auto& kernelLog = KernelLog::Get();
    auto& kernelVmm = vmm::get();

    // Architecture specific setup

    // Step 1: Initialise the early heap with AWD's heap (to save space)
    heap_early_init(awd_info->ptr_malloc_current + (uint64_t)&KERNEL_VMB,
                    awd_info->ptr_malloc_limit + (uint64_t)&KERNEL_VMB);

    // Step 2: Start Logging and VGA
    boot_vga_console_device.Init(0, awd_info->log_cursor_y);
    kernelLog.SetSerialLogging(&boot_serial_logger_device);
    kernelLog.SetTerminalDevice(&boot_vga_console_device);

    // Step 3:  Print the kernel version
    kernel_version_print();

// Step 4: Print build debug info.
#ifdef DEBUG
    kernelLog.Log("debug", "   Kernel VM Base: 0x%016p", (uint64_t)(&KERNEL_VMB));
    kernelLog.Log("debug", "Kernel VM Address: 0x%016p", (uint64_t)(&KERNEL_VMA));
#endif

    // Step 5: Enable Interrupts
    init_idt();

    // Step 6: Initialise memory
    kernelVmm.init(awd_info);                              // Initialise paging structures...
    init_memory((awd_info_t*)MEM_PHYS_TO_VIRT(awd_info));  // Initialise the PMM

    // Architecture specific setup finished, boot into kernel main...
    kernel_main();
}