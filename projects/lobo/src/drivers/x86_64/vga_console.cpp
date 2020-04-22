#include "drivers/x86_64/vga_console.h"

#include <stddef.h>
#include <stdint.h>

#include "x86_64/ports.h"

void VGAConsoleDevice::Init(int curX, int curY) {
    x = curX;
    y = curY;
}

void VGAConsoleDevice::PrintChar(const char c) {
    char* buffer = (char*)(0xB8000);
    switch (c) {
        case '\n':
            x = 0;
            y++;
            break;

        default:
            buffer[((y * width) + x) * 2] = c;
            x++;
            break;
    }

    AttemptScroll();
}

void VGAConsoleDevice::Clear() {
    // TODO: Implement
    x = 0;
    y = 0;
}

void VGAConsoleDevice::AttemptScroll() {
    if(x >= width) {
        y++;
        x = 0;
    }

    if(y >= height) {
        // Scroll
        y--;
    }
}