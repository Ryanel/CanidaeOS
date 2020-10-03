#include <stdio.h>
#include <stdlib.h>
 
#ifdef __is_libk
extern void panic(const char* fmt, ...);
#endif

__attribute__((__noreturn__))
void abort() {
#ifdef __is_libk
	// TODO: Add proper kernel panic.
	panic("abort()");
    while (1) { }
#else
	// TODO: Abnormally terminate the process as if by SIGABRT.
	printf("abort()\n");
#endif
	while (1) { }
	__builtin_unreachable();
}