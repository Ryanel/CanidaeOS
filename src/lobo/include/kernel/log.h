#pragma once

#include <stdarg.h>

namespace kernel {

class IKernelLogTerminalOutDevice;

class log {
   public:
    static log& Get();
    void        WriteChar(const char c);
    void        WriteString(const char* s);
    void        SetSerialLogging(IKernelLogTerminalOutDevice* device);
    void        SetTerminalDevice(IKernelLogTerminalOutDevice* device);
    void        Log(const char* category, const char* fmt, ...);
    void        LogArg(const char* category, const char* fmt, va_list arg);
    void        LogRaw(const char* fmt, ...);

   private:
    int FormatSetLeftColumn(int column);

   private:
    IKernelLogTerminalOutDevice* m_serialOutDevice   = nullptr;
    IKernelLogTerminalOutDevice* m_terminalOutDevice = nullptr;
    bool                         m_backingStore      = false;
    int                          m_fmtLeftColumn     = 0;
};

}  // namespace kernel