#pragma once

#include <stdarg.h>

class IKernelLogTerminalOutDevice {
   public:
    virtual void PrintChar(const char c) = 0;
    virtual void Clear()                 = 0;
};

class KernelLog {
   public:
    static KernelLog& Get();
    void              WriteChar(const char c);
    void              WriteString(const char* s);
    void              SetSerialLogging(IKernelLogTerminalOutDevice* device);
    void              SetTerminalDevice(IKernelLogTerminalOutDevice* device);
    void              Log(const char* category, const char* fmt, ...);

   private:
    IKernelLogTerminalOutDevice* serialOutDevice   = nullptr;
    IKernelLogTerminalOutDevice* terminalOutDevice = nullptr;
    bool                         m_backingStore    = false;
};