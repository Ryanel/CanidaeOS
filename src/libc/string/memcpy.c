#include <string.h>
void* memcpy(void* dest, const void* src, size_t n) {
    unsigned char* destPtr = (unsigned char*)dest;
    unsigned char* srcPtr  = (unsigned char*)src;
    for (size_t i = 0; i < n; i++) {
        *destPtr = *srcPtr;
        destPtr++;
        srcPtr++;
    }

    return dest;
}