#include "kernel/console.h"
#include <stdint.h>
#include "stdio.h"
struct ConsoleProps {
    unsigned int x, y;
    unsigned char color;
    unsigned int width, height;
    uint8_t* buffer;
};

struct ConsoleProps console;

void console_init() {
    console.x = 0;
    console.y = 0;
    console.color = 0x0F;
    console.width = 80;
    console.height = 25;
    console.buffer = (uint8_t*)0xB8000;
}

void console_setcursor(int x, int y) {
    console.x = x;
    console.y = y;
}

void console_writechar(unsigned int x, unsigned int y, char c, unsigned char att) {
    unsigned int position = 2 * ((y * console.width) + x);
    console.buffer[position] = c;
    console.buffer[position + 1] = att;
}

void console_newline() {
    console.y += 1;
    console.x = 0;

    // Hack-y scroll method:

    if (console.y >= console.height) {
        console.y = console.height - 1;
        for (int line = 0; line < (console.height - 1); line++) {
            for (int x = 0; x < console.width; x++) {
                unsigned int position = 2 * (((line + 1) * console.width) + x);
                unsigned int dest = 2 * ((line * console.width) + x);

                char c = console.buffer[position];
                char att = console.buffer[position + 1];

                console.buffer[dest] = c;
                console.buffer[dest + 1] = att;
            }
        }

        // Clear last line
        for (int x = 0; x < console.width; x++) {
            console_writechar(x, console.height - 1, ' ', console.color);
        }
    }
}

void console_color(unsigned char newColor) { console.color = newColor; }

void console_printchar(char c) {
    // Check for formatting characters
    if (c == '\n') {
        console_newline();
    } else {
        console_writechar(console.x, console.y, c, console.color);
        console.x++;
        if (console.x >= console.width) {
            console_newline();
        }
    }
}

void console_printstring(const char* s) {
    int i = 0;
    while (s[i] != '\0') {
        console_printchar(s[i]);
        i++;
    }
}

void console_log(const char* cat, const char* fmt, ...) {
    printf("%8s: ", cat);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}
