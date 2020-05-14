#ifndef _STRING_H
#define _STRING_H 1

#include <stddef.h>
#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t strlen(const char* str);
void*  memset(void* ptr, int value, size_t n);
void*  memcpy(void* dest, const void* src, size_t n);

#ifdef __cplusplus
}
#endif
#endif
