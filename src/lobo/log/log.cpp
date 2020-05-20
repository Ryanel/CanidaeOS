#include <kernel/log.h>

#include <kernel/log_devices.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

using namespace kernel;

static log kernel_log;

log& log::Get() { return kernel_log; }

void log::SetSerialLogging(IKernelLogTerminalOutDevice* device) { m_serialOutDevice = device; }
void log::SetTerminalDevice(IKernelLogTerminalOutDevice* device) { m_terminalOutDevice = device; }

void log::WriteChar(const char c) {
    // Write to serial out
    if (m_serialOutDevice != nullptr) { m_serialOutDevice->PrintChar(c); }
    // Store written character to a backing store
    // TODO: Implement
    if (m_backingStore) {}

    // Write to screen
    if (m_terminalOutDevice != nullptr) { m_terminalOutDevice->PrintChar(c); }
}

void log::WriteString(const char* s) {
    size_t i = 0;
    while (s[i] != '\0') {
        WriteChar(s[i]);
        i++;
    }
}

void log::Log(const char* category, const char* fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    LogArg(category, fmt, arg);
    va_end(arg);
}

void log::LogArg(const char* category, const char* fmt, va_list arg) {
    printf("%9s | ", category);

    FormatSetLeftColumn(13);

    vprintf(fmt, arg);

    FormatSetLeftColumn(0);

    WriteChar('\n');
}

void log::LogRaw(const char* fmt, ...) {
    va_list arg;
    va_start(arg, fmt);

    vprintf(fmt, arg);

    va_end(arg);
}

int log::FormatSetLeftColumn(int column) {
    int old         = m_fmtLeftColumn;
    m_fmtLeftColumn = column;

    if (m_terminalOutDevice != nullptr) { m_terminalOutDevice->FormatSetLeftColumn(column); }
    if (m_serialOutDevice != nullptr) { m_serialOutDevice->FormatSetLeftColumn(column); }

    return old;
}