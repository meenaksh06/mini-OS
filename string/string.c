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

char** str_split(char* str, char delim, int* count) {
    int n = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == delim) n++;
    }
    n++; // number of segments

    char** res = (char**) alloc(n * sizeof(char*));
    int start = 0;
    int seg = 0;

    for (int i = 0; ; i++) {
        if (str[i] == delim || str[i] == '\0') {
            int len = i - start;
            char* segment = (char*) alloc(len + 1);
            for (int k = 0; k < len; k++) {
                segment[k] = str[start + k];
            }
            segment[len] = '\0';
            res[seg++] = segment;
            start = i + 1;
        }
        if (str[i] == '\0') break;
    }

    *count = seg;
    return res;
}