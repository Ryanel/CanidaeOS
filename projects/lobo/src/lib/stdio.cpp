#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kernel/log.h"

static void vprintf_print_padding(KernelLog& log, int length, char* arg,
                                  bool fmt_length_zeropad) {
    if (length != 0) {
        int  charsToPrint = length - strlen(arg);
        char charToPrint  = ' ';
        if (fmt_length_zeropad) { charToPrint = '0'; }
        for (; charsToPrint > 0; charsToPrint--) { log.WriteChar(charToPrint); }
    }
}

void vprintf(const char* fmt, va_list arg) {
    // Cache kernel log reference...
    KernelLog& log = KernelLog::Get();

    // Format variables
    const char* fmt_scan;
    char        fmt_specifier;
    int         fmt_length;
    bool        fmt_length_zeropad = false;
    bool        fmt_left_justify   = false;

    // Argument variables

    unsigned int arg_int;
    uint64_t     arg_int64;
    char*        arg_str;

    // Scan along the format string
    for (fmt_scan = fmt; *fmt_scan != '\0'; fmt_scan++) {
        if (*fmt_scan != '%') {
            log.WriteChar(*fmt_scan);
            continue;
        }

        fmt_length_zeropad = false;
        fmt_left_justify   = false;
        fmt_length         = 0;

        // Skip to the format specifier
        fmt_scan++;

        if (*fmt_scan == '-') {
            fmt_left_justify = true;
            fmt_scan++;
        }

        // Check if we have a length argument
        if (*fmt_scan >= '0' && *fmt_scan <= '9') {
            // We have a length!
            char buffer[8];  // We only handle up to 7 digits
            int  buff_index = 0;
            memset(&buffer, 0, 8);

            fmt_length_zeropad = (*fmt_scan == '0');

            while (*fmt_scan >= '0' && *fmt_scan <= '9' && buff_index < 7) {
                buffer[buff_index++] = *fmt_scan;
                fmt_scan++;
            }

            fmt_length = atoi((const char*)buffer);
        } else {
            fmt_length = 0;
        }

        // Don't allow zero padding when left justifying
        if (fmt_left_justify) { fmt_length_zeropad = false; }

        // Write format specifier
        fmt_specifier = *fmt_scan;

        switch (fmt_specifier) {
            case '%':
                log.WriteChar('%');
                break;

            case 's':
                arg_str = va_arg(arg, char*);
                if (fmt_left_justify) {
                    log.WriteString(arg_str);
                    vprintf_print_padding(log, fmt_length, arg_str, false);
                } else {
                    vprintf_print_padding(log, fmt_length, arg_str, false);
                    log.WriteString(arg_str);
                }
                break;

            case 'c':
                arg_int = va_arg(arg, int);
                log.WriteChar((char)arg_int);
                break;

            case 'd':
                arg_int = va_arg(arg, int);
                arg_str = itoa(arg_int, 10);
                log.WriteString(arg_str);
                break;

            case 'x':
                arg_int = va_arg(arg, int);
                arg_str = itoa(arg_int, 16);
                if (fmt_left_justify) {
                    log.WriteString(arg_str);
                    vprintf_print_padding(log, fmt_length, arg_str,
                                          fmt_length_zeropad);
                } else {
                    vprintf_print_padding(log, fmt_length, arg_str,
                                          fmt_length_zeropad);
                    log.WriteString(arg_str);
                }

                break;

            case 'p':
                arg_int64 = va_arg(arg, uint64_t);
                arg_str   = itoa(arg_int64, 16);
                if (fmt_left_justify) {
                    log.WriteString(arg_str);
                    vprintf_print_padding(log, fmt_length, arg_str,
                                          fmt_length_zeropad);
                } else {
                    vprintf_print_padding(log, fmt_length, arg_str,
                                          fmt_length_zeropad);
                    log.WriteString(arg_str);
                }
                break;

            default:
                log.WriteChar(*fmt_scan);
                break;
        }
    }
}

void printf(const char* fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    vprintf(fmt, arg);
    va_end(arg);
}