#include <stddef.h>
#include <stdint.h>

#include "awd/awd_info.h"
#include "drivers/x86_64/serial.h"
#include "kernel/log.h"
#include "stdio.h"

SerialDevice boot_serial_device;

extern "C" int kernel_entry(awd_info_t* awd_info) {
    awd_info        = awd_info;
    auto& kernelLog = KernelLog::Get();

    kernelLog.SetSerialLogging(&boot_serial_device);
    kernelLog.Log("lobo", "Lobo Kernel [v 0.0.0.1]");
    kernelLog.Log("lobo", "Finished, idling.");

    while (true) { asm("hlt"); }
}