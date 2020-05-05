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

#if DEBUG

void _assert(const char * expression, const char * file, const char * function, int line) {
    KernelLog::Get().Log("assert", "\"%s\" failed in file %s on line %d in function \"%s\"", expression, file, line, function);
    panic("Assertion failed");
}

#endif