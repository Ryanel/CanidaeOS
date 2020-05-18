#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include "stdio.h"

#include "kernel/cpu.h"
#include "kernel/heap.h"
#include "kernel/kernel.h"
#include "kernel/log.h"
#include "kernel/pmm.h"
#include "kernel/scheduler.h"
#include "kernel/vmm.h"
using namespace Kernel;

void kernel_main() {
    auto& kLog        = KernelLog::Get();
    auto& kernelPmm   = Kernel::PMM::Get();
    auto& kernelVmm   = Kernel::VMM::Get();
    auto& kernelSched = Kernel::Scheduling::Scheduler::Get();

    kLog.Log("lobo", "Entered Kernel Main");

    heap_init_full();                  // Initialise the heap
    kernelPmm.DebugPrintFreeMemory();  // Print how much memory was used
    kernelSched.Init();                // Initialise the scheduler



    // Idle thread
    kLog.Log("lobo", "Idle thread");
    CPU::IdleLoop();
}