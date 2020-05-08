#pragma once

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif


void vprintf(const char* fmt, va_list arg);
void printf(const char* fmt, ...);

#ifdef __cplusplus
}
#endif