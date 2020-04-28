#include  <stdio.h>
#include "kernel/log.h"
#include "kernel/cpu.h"


void panic(const char* fmt, ...) {
    va_list arg;

    va_start(arg, fmt);
    KernelLog::Get().Log("panic", fmt, arg);
    va_end(arg);
    
    Kernel::CPU::HaltCPU();
}