#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <stddef.h>
#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

int   atoi(const char* str);
char* itoa(unsigned long n, int base);
void  abort();
#ifdef __cplusplus
}
#endif
#endif
