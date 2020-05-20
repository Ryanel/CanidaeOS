#pragma once

#include <kernel/driver.h>
#include <kernel/log_devices.h>

class UARTLoggingDevice : public kernel::IKernelLogTerminalOutDevice, public kernel::drivers::driver {
   public:
   UARTLoggingDevice() : kernel::drivers::driver("logger_uart", kernel::drivers::driver_type::IN_KERNEL) {}
    bool deviceInit = false;
    void PrintChar(const char c);
    void Clear();
    void FormatSetLeftColumn(int column);

    void init();
    void call();

   private:
    int leftColumn = 0;
};