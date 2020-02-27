#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>
#include <stdint.h>

void *memcpy(void *restrict dstptr, const void *restrict srcptr, size_t size);
int strcmp(const char *str1, const char *str2);

#endif