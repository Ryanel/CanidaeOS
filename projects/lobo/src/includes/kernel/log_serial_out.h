#pragma once
class IKernelLogSerialOutDevice {
   public:
    virtual void PrintChar(const char c) = 0;
};