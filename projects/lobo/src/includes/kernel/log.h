#pragma once

#include <stdarg.h>

#include "kernel/log_serial_out.h"

class KernelLog {
   public:
    static KernelLog& Get();
    void              WriteChar(const char c);
    void              WriteString(const char* s);
    void              SetSerialLogging(IKernelLogSerialOutDevice* device);
    void              Log(const char* category, const char* fmt, ...);

   private:
    IKernelLogSerialOutDevice* serialOutDevice = nullptr;
    bool                       m_backingStore  = false;
};