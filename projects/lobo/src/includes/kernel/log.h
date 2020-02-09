#pragma once

#include "kernel/log_serial_out.h"

class KernelLog {
   public:
    void WriteChar(const char c);
    void WriteString(const char* s);
    void SetSerialLogging(IKernelLogSerialOutDevice* device);

   private:
    IKernelLogSerialOutDevice* serialOutDevice = nullptr;
    bool m_backingStore = false;
};