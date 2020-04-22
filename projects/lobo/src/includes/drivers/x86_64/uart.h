#pragma once
#include "kernel/log.h"

class UARTLoggingDevice : public IKernelLogTerminalOutDevice {
   public:
    bool deviceInit = false;
    void PrintChar(const char c);
    void Clear();
};