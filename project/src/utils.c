#include <string.h>
#include "utils.h"

size_t custom_strlen(const char* str) {
    return strlen(str);
}

int custom_power(int base, int power) {
    if (power == 0) {
        return 1;
    }

    if (power == 1) {
        return base;
    }

    int number_exp = base;
    for (int i = 2; i <= power; ++i) {
        number_exp *= base;
    }

    return number_exp;
}
