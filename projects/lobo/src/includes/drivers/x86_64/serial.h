#pragma once
#include "kernel/log_serial_out.h"

class SerialDevice : public IKernelLogSerialOutDevice {
   public:
    bool deviceInit = false;
    void PrintChar(const char c);
};