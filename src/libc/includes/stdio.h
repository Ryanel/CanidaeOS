#ifndef _STDIO_H
#define _STDIO_H 1

#include <stdarg.h>
#include <stddef.h>
#include <sys/cdefs.h>
#ifdef __cplusplus
extern "C" {
#endif

void vprintf(const char* fmt, va_list arg);
void printf(const char* fmt, ...);

#ifdef __cplusplus
}
#endif
#endif
