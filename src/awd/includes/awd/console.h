#pragma once

#include <stdint.h>

#define CONSOLE_DEFAULT_COLOR 0x0F
#define CONSOLE_WARNING_COLOR 0x0E
#define CONSOLE_ERROR_COLOR   0x4F

void console_init();

void console_write_char(unsigned int x, unsigned int y, char c, unsigned char attribute);
void console_write_serial(char c);

void console_print_char(char c);
void console_print_string(const char* s);

void console_clear();

void console_set_color(unsigned char attr);

int console_get_y();