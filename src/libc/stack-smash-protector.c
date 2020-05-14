#include <stdint.h>
#include <stdlib.h>
 
#if UINT32_MAX == UINTPTR_MAX
#define STACK_CHK_GUARD 0xe2dee396
#else
#define STACK_CHK_GUARD 0x595e9fbd94fda766
#endif
 
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;
 
#ifdef __STDC_HOSTED__
__attribute__((noreturn))
extern void panic(const char* fmt, ...);
#endif

__attribute__((noreturn))
void __stack_chk_fail(void)
{
#ifdef __STDC_HOSTED__
	abort();
#else
	panic("Stack smashing detected");
#endif
    while(1) {}
}