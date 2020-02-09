#include <stddef.h>
#include <stdint.h>

#include "awd/awd_info.h"
#include "drivers/x86_64/serial.h"
#include "kernel/log.h"

extern "C" int kernel_entry(awd_info_t* awd_info) {
    awd_info = awd_info;
    KernelLog kernel_log;
    SerialDevice serial_device;
    kernel_log.SetSerialLogging(&serial_device);
    kernel_log.WriteString("Hello, World!\n");
    while (true) {
    }
}