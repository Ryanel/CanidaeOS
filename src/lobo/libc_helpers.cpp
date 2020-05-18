#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "kernel/log.h"

extern "C" void vprintf_print_padding_helper(int length, char* arg, bool fmt_length_zeropad) {
    auto& log = KernelLog::Get();
    if (length != 0) {
        int  charsToPrint = length - strlen(arg);
        char charToPrint  = ' ';
        if (fmt_length_zeropad) { charToPrint = '0'; }
        for (; charsToPrint > 0; charsToPrint--) { log.WriteChar(charToPrint); }
    }
}

/// C wrapper to print a char to the kernel log from vprintf
extern "C" void vprintf_print_helper(char c) {
    auto& log = KernelLog::Get();
    log.WriteChar(c);
}

/// C wrapper to print a string to the kernel log from vprintf
extern "C" void vprintf_print_helper_string(char * s) {
    auto& log = KernelLog::Get();
    log.WriteString(s);
}

/// For pure virtual functions
extern "C" void __cxa_pure_virtual()
{
}