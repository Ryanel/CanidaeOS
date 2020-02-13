//-----------------------------------------------------------------------------
// console.c
// (C) 2020 Corwin McKnight
// Purpose: Find & Load the ELF64 Kernel, then boot it.
//-----------------------------------------------------------------------------

#include "awd/console.h"

#include <stdarg.h>
#include <stdint.h>

#include "stdio.h"

struct ConsoleProps {
    unsigned int  x, y;
    unsigned char color;
    unsigned int  width, height;
    uint8_t*      buffer;
};

struct ConsoleProps console;

void outb(uint16_t port, uint8_t value) { asm volatile("outb %1, %0" : : "dN"(port), "a"(value)); }

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

void console_init() {
    console.x      = 0;
    console.y      = 0;
    console.color  = 0x0F;
    console.width  = 80;
    console.height = 25;
    console.buffer = (uint8_t*)0xB8000;

    // Init serial
    const uint16_t port = 0x3f8;

    outb(port + 1, 0x00);
    outb(port + 3, 0x80);
    outb(port + 0, 0x03);
    outb(port + 1, 0x00);
    outb(port + 3, 0x03);
    outb(port + 2, 0xC7);
    outb(port + 4, 0x0B);
}

void console_write_serial(const char c) {
    const uint16_t port = 0x3f8;

    while ((inb(port + 5) & 0x20) == 0) {}

    outb(port, c);

    // Always print a carridge return on newline
    if (c == '\n') { console_write_serial('\r'); }
}

void console_writechar(unsigned int x, unsigned int y, char c, unsigned char att) {
    unsigned int position        = 2 * ((y * console.width) + x);
    console.buffer[position]     = c;
    console.buffer[position + 1] = att;
}

void console_scroll() {
    for (unsigned int line = 0; line < (console.height - 1); line++) {
        for (unsigned int x = 0; x < console.width; x++) {
            unsigned int src  = 2 * (((line + 1) * console.width) + x);
            unsigned int dest = 2 * ((line * console.width) + x);

            char c   = console.buffer[src];
            char att = console.buffer[src + 1];

            console.buffer[dest]     = c;
            console.buffer[dest + 1] = att;
        }
    }
    // Clear last line
    for (unsigned int x = 0; x < console.width; x++) { console_writechar(x, console.height - 1, ' ', console.color); }
}

void console_newline() {
    console.y += 1;
    console.x = 0;

    if (console.y >= console.height) {
        console.y = console.height - 1;
        console_scroll();
    }
}

void console_printchar(char c) {
    if (c == '\n') {
        console_newline();
    } else {
        console_writechar(console.x, console.y, c, console.color);
        console.x++;
        if (console.x >= console.width) { console_newline(); }
    }

    console_write_serial(c);
}

void console_printstring(const char* s) {
    int i = 0;
    while (s[i] != '\0') {
        console_printchar(s[i]);
        i++;
    }
}

void console_color(unsigned char newColor) { console.color = newColor; }

unsigned int console_getX() { return console.x; }

unsigned int console_getY() { return console.y; }

void console_log(const char* cat, const char* fmt, ...) {
    printf("%5s: ", cat);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void console_print_uint64(uint64_t tp) {
    uint32_t vaddr_lo = tp & 0xFFFFFFFF;
    uint32_t vaddr_hi = tp >> 32;

    printf("0x%08X%08X", vaddr_hi, vaddr_lo);
}