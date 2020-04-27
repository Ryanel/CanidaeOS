#include <stddef.h>
#include <stdint.h>
#include "stdio.h"

#include "kernel/log.h"

void kernel_idle_thread() {
    auto& kLog = KernelLog::Get();

    kLog.Log("lobo", "Entered Kernel Idle Thread");

    while (true) {}
}

void kernel_main() {
    auto& kLog = KernelLog::Get();
    
    kLog.Log("lobo", "Entered Kernel Main");
    
    uint8_t * cause_pf = (uint8_t *)0xA000000;

    cause_pf[12] = 0xA;

    kernel_idle_thread();
}
