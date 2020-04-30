#pragma once

#include <stdarg.h>

class IKernelLogTerminalOutDevice {
   public:
    virtual void PrintChar(const char c) {}
    virtual void Clear() {}
};

class KernelLog {
   public:
    static KernelLog& Get();
    void              WriteChar(const char c);
    void              WriteString(const char* s);
    void              SetSerialLogging(IKernelLogTerminalOutDevice* device);
    void              SetTerminalDevice(IKernelLogTerminalOutDevice* device);
    void              Log(const char* category, const char* fmt, ...);
    void              LogArg(const char* category, const char* fmt, va_list arg);
    void              LogRaw(const char* fmt, ...);

   private:
    IKernelLogTerminalOutDevice* serialOutDevice   = nullptr;
    IKernelLogTerminalOutDevice* terminalOutDevice = nullptr;
    bool                         m_backingStore    = false;
};