#include "kernel/log.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

static KernelLog kernel_log;

KernelLog& KernelLog::Get() { return kernel_log; }
void KernelLog::SetSerialLogging(IKernelLogTerminalOutDevice* device) { serialOutDevice = device; }
void KernelLog::SetTerminalDevice(IKernelLogTerminalOutDevice* device) { terminalOutDevice = device; }

void KernelLog::WriteChar(const char c) {
    // Store written character to backing store
    if (m_backingStore) {}
    
    // Write to serial out
    if (serialOutDevice != nullptr) { serialOutDevice->PrintChar(c); }

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
    printf("%5s: ", category);

    va_list arg;
    va_start(arg, fmt);
    vprintf(fmt, arg);
    va_end(arg);

    WriteChar('\n');
}