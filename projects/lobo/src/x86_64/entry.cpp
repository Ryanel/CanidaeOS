#include <stddef.h>
#include <stdint.h>

#include "awd/awd_info.h"
#include "drivers/x86_64/serial.h"
#include "kernel/log.h"
#include "stdio.h"

SerialDevice boot_serial_device;

extern "C" int kernel_entry(awd_info_t* awd_info) {
    awd_info = awd_info;

    KernelLog::Get().SetSerialLogging(&boot_serial_device);

    printf("0.00000 | lobo: Lobo Kernel [v 0.0.0.1]\n");
    printf("0.00000 | lobo: Finished, idling.\n");

    while (true) {}
}