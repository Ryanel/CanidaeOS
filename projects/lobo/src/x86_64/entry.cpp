#include <stddef.h>
#include <stdint.h>

#include "awd/awd_info.h"
#include "drivers/x86_64/serial.h"
#include "kernel/log.h"
#include "stdio.h"

extern "C" int kernel_entry(awd_info_t* awd_info) {
    awd_info = awd_info;

    SerialDevice serial_device;

    KernelLog::Get().SetSerialLogging(&serial_device);
    KernelLog::Get().WriteString("Hello, World!\n");

    printf("Hello! %%d: %d %%s: %s %%x: 0x%08x\n", 123, "Test String",
           0xABCDFEDC);
    printf("Hello! A: %%p test= 0x%p\n", 0xFFFFFFFF800000AB);

    KernelLog::Get().WriteString("[idling]\n");
    while (true) {}
}