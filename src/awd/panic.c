#include <stdarg.h>
#include <stdio.h>

#include <awd/awd.h>
#include <awd/console.h>

void panic(const char* fmt, ...) {
    console_set_color(CONSOLE_ERROR_COLOR);
    va_list arg;
    va_start(arg, fmt);
    vprintf(fmt, arg);
    va_end(arg);

    awd_halt();
}