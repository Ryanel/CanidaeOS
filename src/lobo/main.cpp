#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include "stdio.h"

#include <kernel/debug/kernel_debugger.h>
#include <kernel/driver_registry.h>
#include <kernel/mem/memmap.h>
#include "kernel/cpu.h"
#include "kernel/heap.h"
#include "kernel/kernel.h"
#include "kernel/log.h"
#include "kernel/pmm.h"
#include "kernel/scheduler.h"
#include "kernel/vmm.h"
#include <kernel/driver.h>
using namespace kernel;

void test_thread() {
    auto& kLog                 = log::Get();
    while(true) {
        kLog.Log("lobo", "Test A");
    }
    
}

void test_thread2() {
    auto& kLog                 = log::Get();
    while(true) {
        kLog.Log("lobo", "Test B");
    }
}


void kernel_main() {
    auto& kLog                 = log::Get();
    auto& kernelPmm            = kernel::pmm::get();
    auto& kernelSched          = kernel::scheduling::Scheduler::Get();

    kLog.Log("lobo", "Entered Kernel Main");

    kernelPmm.debug_print_free_memory();  // Print how much memory was used
    kernelSched.Init();                   // Initialise the scheduler

    //debug::debugger().get().enter();

    kernelSched.CreateThread("k/test", (void*)test_thread);
    kernelSched.CreateThread("k/test2", (void*)test_thread2);

    kernelSched.EnableScheduling();

    kLog.Log("lobo", "Entered idle thread");
    while(true) {
        kLog.Log("lobo", "Idle");
    }
    cpu::IdleLoop();
}
