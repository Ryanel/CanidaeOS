#include <stdlib.h>

int atoi(const char* str) {
    int result = 0;

    for (size_t i = 0; str[i] != '\0'; i++) {
        result = (result * 10) + (str[i] - '0');
    }

    return result;
}