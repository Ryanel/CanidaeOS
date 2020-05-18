#include "kernel/arch/x86_64/drivers/uart.h"

#include <stddef.h>
#include <stdint.h>

#include "kernel/arch/x86_64/ports.h"

void UARTLoggingDevice::PrintChar(const char c) {
    const uint16_t port = 0x3f8;
    if (!deviceInit) {
        outb(port + 1, 0x00);
        outb(port + 3, 0x80);
        outb(port + 0, 0x03);
        outb(port + 1, 0x00);
        outb(port + 3, 0x03);
        outb(port + 2, 0xC7);
        outb(port + 4, 0x0B);
        deviceInit = true;
    }

    while ((inb(port + 5) & 0x20) == 0) {}

    outb(port, c);

    // Always print a carridge return on newline
    if (c == '\n') { 
        PrintChar('\r'); 
        for (int i = 0; i < leftColumn; i++) {
            PrintChar(' ');
        }
    }
}

void UARTLoggingDevice::Clear() {
    // TODO: Implement
}


void UARTLoggingDevice::FormatSetLeftColumn(int column) {
    // Does nothing
    leftColumn = column;
}