#pragma once
#include <stddef.h>

size_t strlen(const char* str);
void*  memset(void* ptr, int value, size_t n);
void*  memcpy(void* dest, const void* src, size_t n);