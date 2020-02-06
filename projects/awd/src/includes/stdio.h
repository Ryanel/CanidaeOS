#ifndef _STDIO_H
#define _STDIO_H 1

#include <stdarg.h>

#define EOF (-1)

int printf(const char *__restrict, ...);
int putchar(int);
int puts(const char *);

int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);
int vprintf(const char *fmt, va_list args);
int printf(const char *fmt, ...);

#endif