#pragma once
#include <stdint.h>
// Include kernel configuration
#include <kernel/kconfig.h>

// Linker symbols, take the address of them.
extern "C" uint64_t KERNEL_VMB;
extern "C" uint64_t KERNEL_VMA;
extern "C" uint64_t _kernel_start;
extern "C" uint64_t _kernel_end;
extern "C" uint64_t _kernel_ro_start;
extern "C" uint64_t _kernel_ro_end;
extern "C" uint64_t _kernel_data_start;
extern "C" uint64_t _kernel_data_end;

// Memory conversion macros to make things easier.
#define MEM_VIRT_TO_PHYS(a) ((uint64_t)((uint64_t)a - ((uint64_t)&KERNEL_VMB)))
#define MEM_PHYS_TO_VIRT(a) ((uint64_t)((uint64_t)a + ((uint64_t)&KERNEL_VMB)))

