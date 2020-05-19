#include  <stdio.h>
#include "kernel/log.h"
#include "kernel/cpu.h"

using namespace kernel;

extern "C" void panic(const char* fmt, ...) {
    va_list arg;

    va_start(arg, fmt);
    log::Get().Log("panic", fmt, arg);
    va_end(arg);
    
    cpu::HaltCPU();
}

#if DEBUG

void _assert(const char * expression, const char * file, const char * function, int line) {
    log::Get().Log("assert", "\"%s\" failed\nIn file %s, on line %d\nIn function \"%s\"", expression, file, line, function);
    panic("Assertion failed");
}

#endif