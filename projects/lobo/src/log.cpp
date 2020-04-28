#include "kernel/log.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

static KernelLog kernel_log;

KernelLog& KernelLog::Get() { return kernel_log; }
void KernelLog::SetSerialLogging(IKernelLogTerminalOutDevice* device) { serialOutDevice = device; }
void KernelLog::SetTerminalDevice(IKernelLogTerminalOutDevice* device) { terminalOutDevice = device; }

void KernelLog::WriteChar(const char c) {
    // Write to serial out
    if (serialOutDevice != nullptr) { serialOutDevice->PrintChar(c); }

    // Store written character to a backing store
    // TODO: Implement
    if (m_backingStore) {}
    
    // Write to screen
    if (terminalOutDevice != nullptr) { terminalOutDevice->PrintChar(c); }
}

void KernelLog::WriteString(const char* s) {
    size_t i = 0;
    while (s[i] != '\0') {
        WriteChar(s[i]);
        i++;
    }
}

void KernelLog::Log(const char* category, const char* fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    LogArg(category, fmt, arg);
    va_end(arg);
}

void KernelLog::LogArg(const char* category, const char* fmt, va_list arg) {
    printf("%6s: ", category);
    vprintf(fmt, arg);
    WriteChar('\n');
}

void KernelLog::LogRaw(const char* fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    vprintf(fmt, arg);
    va_end(arg);
}