#include <stdarg.h>
#include <stdio.h>

#include <awd/console.h>

void log_info(const char* fmt, ...) {
    console_set_color(CONSOLE_DEFAULT_COLOR);
    va_list arg;
    va_start(arg, fmt);
    vprintf(fmt, arg);
    va_end(arg);
}

void log_notice(const char* fmt, ...) {
    console_set_color(CONSOLE_DEFAULT_COLOR);
    va_list arg;
    va_start(arg, fmt);
    vprintf(fmt, arg);
    va_end(arg);
}

void log_error(const char* fmt, ...) {
    console_set_color(CONSOLE_ERROR_COLOR);
    va_list arg;
    va_start(arg, fmt);
    vprintf(fmt, arg);
    va_end(arg);
}