#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "kernel/log.h"

int atoi(const char* str) {
    int result = 0;

    for (size_t i = 0; str[i] != '\0'; i++) {
        result = (result * 10) + (str[i] - '0');
    }

    return result;
}

char* itoa(uint64_t n, int base) {
    const char  lut[] = "0123456789ABCDEF";
    static char buffer[50];

    char* buf_index = &buffer[49];
    *buf_index      = '\0';

    do {
        *--buf_index = lut[n % base];
        n /= base;
    } while (n > 0);
    return (char*)buf_index;
}

void printf(char* fmt, ...) {
    // Cache kernel log reference...
    KernelLog& log = KernelLog::Get();

    // Format variables
    char* fmt_scan;
    char  fmt_specifier;
    int   fmt_length;
    bool  fmt_length_zeropad = false;

    // Argument variables
    va_list      arg;
    unsigned int arg_int;
    uint64_t     arg_int64;
    char*        arg_str;

    va_start(arg, fmt);

    // Scan along the format string
    for (fmt_scan = fmt; *fmt_scan != '\0'; fmt_scan++) {
        if (*fmt_scan != '%') {
            log.WriteChar(*fmt_scan);
            continue;
        }

        // Skip to the format specifier
        fmt_scan++;

        // Check if we have a length argument
        if (*fmt_scan >= '0' && *fmt_scan <= '9') {
            // We have a length!
            char buffer[8];  // We only handle up to 7 digits
            int  buff_index = 0;

            buffer[7] = 0;  // Ensure null byte

            fmt_length_zeropad = (*fmt_scan == '0');

            while (*fmt_scan >= '0' && *fmt_scan <= '9' && buff_index < 7) {
                buffer[buff_index] = *fmt_scan;
                log.WriteChar(*fmt_scan);
                fmt_scan++;
            }

            fmt_length = atoi((const char*)buffer);
        }

        // Write format specifier
        fmt_specifier = *fmt_scan;

        switch (fmt_specifier) {
            case '%':
                log.WriteChar('%');
                break;

            case 'd':
                arg_int = va_arg(arg, int);
                log.WriteString(itoa(arg_int, 10));
                break;

            case 'x':
                arg_int = va_arg(arg, int);
                log.WriteString(itoa(arg_int, 16));
                break;

            case 'p':
                arg_int64 = va_arg(arg, uint64_t);
                log.WriteString(itoa(arg_int64, 16));
                break;

            case 's':
                arg_str = va_arg(arg, char*);
                log.WriteString(arg_str);
                break;

            case 'c':
                arg_int = va_arg(arg, int);
                log.WriteChar((char)arg_int);
                break;

            default:
                log.WriteChar(*fmt_scan);
                break;
        }
    }

    va_end(arg);
}