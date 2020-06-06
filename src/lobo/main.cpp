#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include "stdio.h"

#include <kernel/debug/kernel_debugger.h>
#include <kernel/driver.h>
#include <kernel/driver_registry.h>
#include <kernel/mem/memmap.h>
#include "kernel/cpu.h"
#include "kernel/heap.h"
#include "kernel/kernel.h"
#include "kernel/log.h"
#include "kernel/pmm.h"
#include "kernel/scheduler.h"
#include "kernel/vmm.h"
using namespace kernel;

void test_thread() {
    auto& kLog = log::Get();
    while (true) {
        kLog.Log("lobo", "Entered k/test");
        debug::debugger().get().enter();
        kernel::scheduling::Scheduler::Get().Yield();
    }
}

void test_thread2() {
    auto& kLog = log::Get();
    while (true) {
        kLog.Log("lobo", "Entered k/test2");
        debug::debugger().get().enter();
        kernel::scheduling::Scheduler::Get().Yield();
    }
}

void test_thread3() {
    auto& kLog = log::Get();
    while (true) {
        kLog.Log("lobo", "Entered k/test_dead");
        debug::debugger().get().enter();
        kernel::scheduling::Scheduler::Get().Yield();
    }
}
void test_thread4() {
    auto& kLog = log::Get();
    while (true) {
        kLog.Log("lobo", "Entered k/test_waiting");
        debug::debugger().get().enter();
        kernel::scheduling::Scheduler::Get().Yield();
    }
}
void kernel_main() {
    auto& kLog        = log::Get();
    auto& kernelPmm   = kernel::pmm::get();
    auto& kernelSched = kernel::scheduling::Scheduler::Get();

    kLog.Log("lobo", "Entered Kernel Main");

    kernelPmm.debug_print_free_memory();  // Print how much memory was used
    kernelSched.Init();                   // Initialise the scheduler

    debug::debugger().get().enter();

    kernelSched.CreateThread("k/test", (void*)test_thread);
    kernelSched.CreateThread("k/test2", (void*)test_thread2);
    
    auto* deadThread = kernelSched.CreateThread("k/test_dead", (void*)test_thread3);
    deadThread->state = scheduling::TCBState::Dead;

    auto* waitingThread = kernelSched.CreateThread("k/test_waiting", (void*)test_thread4);
    waitingThread->state = scheduling::TCBState::Waiting;

    kernelSched.EnableScheduling();

    kLog.Log("lobo", "Entered idle thread");

    kernelSched.Yield();
    debug::debugger().get().enter();

    cpu::IdleLoop();
}
