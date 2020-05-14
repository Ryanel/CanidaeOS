#include <stdlib.h>

char* itoa(unsigned long n, int base) {
    const char  lut[] = "0123456789ABCDEF";
    static char buffer[sizeof(unsigned long) * 8 + 1];

    char* buf_index = &buffer[49];
    *buf_index      = '\0';

    do {
        *--buf_index = lut[n % base];
        n /= base;
    } while (n > 0);
    return (char*)buf_index;
}