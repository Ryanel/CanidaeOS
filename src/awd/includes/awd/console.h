#ifndef AWD_CONSOLE_H
#define AWD_CONSOLE_H

#include <stdint.h>

/// Initialises the console
void console_init();

/// Writes a character a (x,y) with c as the character to print, with the color att
void console_writechar(unsigned int x, unsigned int y, char c, unsigned char att);

/// Prints a character to the screen
void console_printchar(char c);

/// Prints a string to the screen
void console_printstring(const char* s);

/// Change the color
void console_color(unsigned char newColor);

/// Get X coordinate
unsigned int console_getX();

/// Get Y coordinate
unsigned int console_getY();

/// Log to screen in a nice formatted way
void console_log(const char* cat, const char* fmt, ...);

/// Print a 64-bit number correctly
void console_print_uint64(uint64_t tp);

#endif  // AWD_CONSOLE_H