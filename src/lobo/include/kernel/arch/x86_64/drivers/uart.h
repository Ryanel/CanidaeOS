#pragma once
#include "kernel/log.h"

class UARTLoggingDevice : public kernel::IKernelLogTerminalOutDevice {
   public:
    bool deviceInit = false;
    void PrintChar(const char c);
    void Clear();
    void FormatSetLeftColumn(int column);

    private:
    int leftColumn = 0;
};