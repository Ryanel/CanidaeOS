#include <stddef.h>
#include <stdint.h>

int atoi(const char* str) {
    int result = 0;

    for (size_t i = 0; str[i] != '\0'; i++) {
        result = (result * 10) + (str[i] - '0');
    }

    return result;
}

char* itoa(uint64_t n, int base) {
    const char  lut[] = "0123456789ABCDEF";
    static char buffer[50];

    char* buf_index = &buffer[49];
    *buf_index      = '\0';

    do {
        *--buf_index = lut[n % base];
        n /= base;
    } while (n > 0);
    return (char*)buf_index;
}