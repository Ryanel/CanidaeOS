#include <string.h>
void* memset(void* ptr, int value, size_t num) {
    unsigned char * ptr_index = (unsigned char *)ptr;
    for (size_t i = 0; i < num; i++) {
        *ptr_index = (unsigned char)value;
        ptr_index++;
    }

    return ptr;
}