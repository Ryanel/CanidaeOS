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