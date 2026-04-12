#include "string.h"
#include "../memory/memory.h"

int str_len(char* str) {
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

void str_copy(char* dest, char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int str_compare(char* a, char* b) {
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

int str_to_int(char* str) {
    int num = 0;
    int i = 0;

    while (str[i] != '\0') {
        num = num * 10 + (str[i] - '0');
        i++;
    }

    return num;
}

char* int_to_str(int num) {
    char* str = (char*) alloc(20);

    int i = 0;
    int temp = num;

    if (temp == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    while (temp > 0) {
        str[i++] = (temp % 10) + '0';
        temp /= 10;
    }

    str[i] = '\0';

    // reverse
    for (int j = 0; j < i / 2; j++) {
        char t = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = t;
    }

    return str;
}