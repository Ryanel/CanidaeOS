#include <stddef.h>
#include <stdint.h>
#include "stdio.h"

#include "kernel/log.h"
#include "kernel/pmm.h"
#include "kernel/cpu.h"

using namespace Kernel;

void kernel_idle_thread() {
    auto& kLog = KernelLog::Get();
    kLog.Log("lobo", "Entered Kernel Idle Thread");
    CPU::IdleLoop();
}

void kernel_main() {
    auto& kLog = KernelLog::Get();
    
    kLog.Log("lobo", "Entered Kernel Main");
    auto& kernelPmm =  Kernel::PMM::Get();
    kernelPmm.DebugPrintFreePages();

    kernel_idle_thread();
}
