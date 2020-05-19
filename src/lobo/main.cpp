#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include "stdio.h"

#include <kernel/mem/memmap.h>
#include "kernel/cpu.h"
#include "kernel/heap.h"
#include "kernel/kernel.h"
#include "kernel/log.h"
#include "kernel/pmm.h"
#include "kernel/scheduler.h"
#include "kernel/vmm.h"
using namespace kernel;

void kernel_main() {
    auto& kLog        = log::Get();
    auto& kernelPmm   = kernel::pmm::get();
    auto& kernelVmm   = kernel::vmm::get();
    auto& kernelSched = kernel::scheduling::Scheduler::Get();

    kLog.Log("lobo", "Entered Kernel Main"); 
    kernelPmm.debug_print_free_memory();  // Print how much memory was used
    kernelSched.Init();                   // Initialise the scheduler

    // Idle thread
    kLog.Log("lobo", "Idle thread");
    cpu::IdleLoop();
}