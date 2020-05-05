#include "drivers/x86_64/vga_console.h"

#include <stddef.h>
#include <stdint.h>

#include "x86_64/ports.h"
#include "kernel/kernel.h"


void VGAConsoleDevice::Init(int curX, int curY) {
    x = curX;
    y = curY;
    bufferAddr = MEM_PHYS_TO_VIRT(0xB8000); // TODO: Convert to virtual
}

void VGAConsoleDevice::PrintChar(const char c) {
    char*        buffer = (char*)(bufferAddr);
    unsigned int offset = ((y * width) + x) * 2;
    switch (c) {
        case '\n':
            x = leftColumn;
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
        x = leftColumn;
    }

    if (y >= height) {
        // Scroll
        char*  buffer = (char*)(bufferAddr);

        for(int yi = 1; yi < height; yi++) {
            for(int xi = 0; xi < width; xi++) {
                unsigned int offsetI = ((yi * width) + xi) * 2;
                unsigned int offsetP = (((yi - 1) * width) + xi) * 2;

                buffer[offsetP] = buffer[offsetI];
                buffer[offsetP + 1] = buffer[offsetI + 1];
            }
        }
        y--;

        int yi = height - 1;
        for(int xi = 0; xi < width; xi++) {
            unsigned int offsetI = ((yi * width) + xi) * 2;
             buffer[offsetI] = ' ';
        }
    }
}

void VGAConsoleDevice::FormatSetLeftColumn(int column) {
    leftColumn = column;
}