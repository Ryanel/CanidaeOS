#ifndef CONSOLE_H
#define CONSOLE_H

/// Initialises the console
void console_init();

/// Writes a character a (x,y) with c as the character to print,
/// with the color att
void console_writechar(unsigned int x, unsigned int y, char c, unsigned char att);

/// Prints a character to the screen
void console_printchar(char c);
/// Prints a string to the screen
void console_printstring(const char* s);
/// Change the color
void console_color(unsigned char newColor);
void console_setcursor(int x, int y);
/// Log to screen in a nice formatted way
void console_log(const char* cat, const char* fmt, ...);
#endif  // CONSOLE_H