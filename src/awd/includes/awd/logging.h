#pragma once

#include <stdarg.h>

void log_info(const char* fmt, ...);
void log_notice(const char* fmt, ...);
void log_error(const char* fmt, ...);