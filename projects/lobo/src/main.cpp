#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include "stdio.h"

#include "kernel/cpu.h"
#include "kernel/heap.h"
#include "kernel/log.h"
#include "kernel/pmm.h"
#include "kernel/vmm.h"

using namespace Kernel;

void kernel_idle_thread() {
    auto& kLog = KernelLog::Get();
    kLog.Log("lobo", "Entered Kernel Idle Thread");
    CPU::IdleLoop();
}

void kernel_main() {
    auto& kLog      = KernelLog::Get();
    auto& kernelPmm = Kernel::PMM::Get();
    auto& kernelVmm = Kernel::VMM::Get();

    kLog.Log("lobo", "Entered Kernel Main");

    kernelPmm.DebugPrintFreeMemory();

    kernelVmm.Map((void*)0xA00000, 1, 0, 0);

    kernel_idle_thread();
}
