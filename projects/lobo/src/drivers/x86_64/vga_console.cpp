#include "drivers/x86_64/vga_console.h"

#include <stddef.h>
#include <stdint.h>

#include "x86_64/ports.h"

void VGAConsoleDevice::Init(int curX, int curY) {
    x = curX;
    y = curY;
}

void VGAConsoleDevice::PrintChar(const char c) {
    char*        buffer = (char*)(0xB8000);
    unsigned int offset = ((y * width) + x) * 2;
    switch (c) {
        case '\n':
            x = 0;
            y++;
            break;

        default:
            buffer[offset]     = c;
            buffer[offset + 1] = attribute;
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
    if (x >= width) {
        y++;
        x = 0;
    }

    if (y >= height) {
        // Scroll

        char*  buffer = (char*)(0xB8000);

        for(int yi = 1; yi < height; yi++) {
            for(int xi = 0; xi < width; xi++) {
                unsigned int offsetI = ((yi * width) + xi) * 2;
                unsigned int offsetP = (((yi - 1) * width) + xi) * 2;

                buffer[offsetP] = buffer[offsetI];
                buffer[offsetP + 1] = buffer[offsetI + 1];
            }
        }
        y--;
    }
}