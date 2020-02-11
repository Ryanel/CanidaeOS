#include <stdint.h>
#include <string.h>

size_t strlen(const char* str) {
    char*  idx    = (char*)str;
    size_t length = 0;
    while (*idx != '\0') {
        idx++;
        length++;
    }
    return length;
}

// TODO: Slow and unoptimized
void* memset(void* ptr, int value, size_t num) {
    uint8_t* ptr_index = (uint8_t*)ptr;
    for (size_t i = 0; i < num; i++) {
        *ptr_index = (unsigned char)value;
        ptr_index++;
    }

    return ptr;
}

// TODO: Slow and unoptimized
void* memcpy(void* dest, const void* src, size_t n) {
    uint8_t*       destPtr = (uint8_t*)dest;
    const uint8_t* srcPtr  = (const uint8_t*)src;
    for (size_t i = 0; i < n; i++) {
        *destPtr = *srcPtr;
        destPtr++;
        srcPtr++;
    }

    return dest;
}