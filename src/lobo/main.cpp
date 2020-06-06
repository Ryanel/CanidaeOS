#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include "stdio.h"

#include <kernel/debug/kernel_debugger.h>
#include <kernel/driver.h>
#include <kernel/driver_registry.h>
#include <kernel/hal/keyboard.h>
#include <kernel/mem/memmap.h>
#include "kernel/cpu.h"
#include "kernel/heap.h"
#include "kernel/kernel.h"
#include "kernel/log.h"
#include "kernel/pmm.h"
#include "kernel/scheduler.h"
#include "kernel/vmm.h"

using namespace kernel;

void thread_read_keyboard() {
    auto& kLog = log::Get();
    while (true) {
        char c = kernel::hal::keyboard::get().getchar_async();
        if (c == '~') {
            kernel::debug::debugger::get().enter();
        } else if (c != 0) {
            kLog.WriteChar(c);
        }
    }
}

void test_thread2() {
    auto& kLog = log::Get();
    while (true) {
        // kLog.Log("lobo", "Entered k/test2");
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

    // Create test threads
    kernelSched.CreateThread("k/keyboard", (void*)thread_read_keyboard);
    kernelSched.CreateThread("k/test2", (void*)test_thread2);
    auto* deadThread     = kernelSched.CreateThread("k/test_dead", (void*)test_thread3);
    auto* waitingThread  = kernelSched.CreateThread("k/test_waiting", (void*)test_thread4);
    deadThread->state    = scheduling::TCBState::Dead;
    waitingThread->state = scheduling::TCBState::Waiting;

    kernelSched.EnableScheduling();

    kLog.Log("lobo", "Entered idle thread");

    // debug::debugger().get().enter();
    cpu::IdleLoop();
}
