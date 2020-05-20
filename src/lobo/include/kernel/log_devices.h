#pragma once

namespace kernel {
    
class IKernelLogTerminalOutDevice {
   public:
    virtual void PrintChar(const char c) {}
    virtual void Clear() {}
    virtual void FormatSetLeftColumn(int column);
};

}  // namespace kernel