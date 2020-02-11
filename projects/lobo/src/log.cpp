#include "kernel/log.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

static KernelLog kernel_log;

KernelLog& KernelLog::Get() { return kernel_log; }

void KernelLog::WriteChar(const char c) {
    // Write to serial out
    if (serialOutDevice != nullptr) { serialOutDevice->PrintChar(c); }

    // Store written character to backing store
    if (m_backingStore) {}
}
void KernelLog::WriteString(const char* s) {
    size_t i = 0;
    while (s[i] != '\0') {
        WriteChar(s[i]);
        i++;
    }
}
void KernelLog::SetSerialLogging(IKernelLogSerialOutDevice* device) {
    serialOutDevice = device;
}

void KernelLog::Log(const char* category, const char* fmt, ...) {
    // TODO: Hook up timing subsystem to kernel log output
    printf("00.00000 | %5s: ", category);

    va_list arg;
    va_start(arg, fmt);
    vprintf(fmt, arg);
    va_end(arg);

    WriteChar('\n');
}