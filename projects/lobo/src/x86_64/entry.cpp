#include <stddef.h>
#include <stdint.h>
#include "stdio.h"

#include "awd/awd_info.h"
#include "drivers/x86_64/uart.h"
#include "drivers/x86_64/vga_console.h"
#include "kernel/log.h"
#include "x86_64/interrupts.h"

void kernel_main();
void kernel_version_print();

UARTLoggingDevice boot_serial_logger_device;
VGAConsoleDevice  boot_vga_console_device;

extern "C" void kernel_entry(awd_info_t* awd_info) {
    awd_info        = awd_info;
    auto& kernelLog = KernelLog::Get();

    // Architecture specific setup
    // Start VGA and Logging
    boot_vga_console_device.Init(0, awd_info->log_cursor_y);
    kernelLog.SetSerialLogging(&boot_serial_logger_device);
    kernelLog.SetTerminalDevice(&boot_vga_console_device);

    // Print the kernel version
    kernel_version_print();

    // Interrupts
    kernelLog.Log("int", "Initialising interrupts");
    init_idt();
    kernelLog.Log("int", "Interrupts enabled");

    // Architecture specific setup finished, boot into kernel main...
    kernel_main();
}