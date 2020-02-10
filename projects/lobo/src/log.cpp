#include "kernel/log.h"

#include <stddef.h>

static KernelLog kernel_log;

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

KernelLog& KernelLog::Get() { return kernel_log; }