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

void kernel_a_thread() {
    auto& kLog = KernelLog::Get();
    while (true) { kLog.LogRaw("A"); }
}

void kernel_b_thread() {
    auto& kLog = KernelLog::Get();
    while (true) { kLog.LogRaw("B"); }
}

void kernel_c_thread() {
    auto& kLog = KernelLog::Get();
    while (true) { kLog.LogRaw("C"); }
}

void kernel_main() {
    auto& kLog        = KernelLog::Get();
    auto& kernelPmm   = Kernel::PMM::Get();
    auto& kernelVmm   = Kernel::VMM::Get();
    auto& kernelSched = Kernel::Scheduler::Get();

    kLog.Log("lobo", "Entered Kernel Main");

    heap_init_full();                  // Initialise the heap
    kernelPmm.DebugPrintFreeMemory();  // Print how much memory was used
    kernelSched.Init();                // Initialise the scheduler
    /*
    kernelSched.EnableScheduling();
    ThreadControlBlock * thread_a = kernelSched.CreateThread("k/worker_a", (void*)kernel_a_thread);
    ThreadControlBlock * thread_b = kernelSched.CreateThread("k/worker_b", (void*)kernel_b_thread);
    ThreadControlBlock * thread_c = kernelSched.CreateThread("k/worker_c", (void*)kernel_c_thread);

    thread_a->next = thread_b;
    thread_b->next = thread_c;
    thread_c->next = thread_a;

    kernelSched.SwitchThread(thread_a);
    */

    // Idle thread
    kLog.Log("lobo", "Idle thread");
    CPU::IdleLoop();
}
