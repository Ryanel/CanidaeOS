#include <stddef.h>
#include <stdint.h>
#include "stdio.h"

#include "awd/awd_info.h"
#include "drivers/x86_64/uart.h"
#include "drivers/x86_64/vga_console.h"
#include "kernel/log.h"

UARTLoggingDevice boot_serial_logger_device;
VGAConsoleDevice  boot_vga_console_device;

extern "C" int kernel_entry(awd_info_t* awd_info) {
    awd_info        = awd_info;
    auto& kernelLog = KernelLog::Get();

    boot_vga_console_device.Init(0, awd_info->log_cursor_y);

    kernelLog.SetSerialLogging(&boot_serial_logger_device);
    kernelLog.SetTerminalDevice(&boot_vga_console_device);

    kernelLog.Log("lobo", "Lobo Kernel [v 0.0.0.1]");
    kernelLog.Log("lobo", "Finished, idling.");

    while (true) { asm("hlt"); }
}