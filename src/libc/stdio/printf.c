#include <stdio.h>

void printf(const char* fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    vprintf(fmt, arg);
    va_end(arg);
}