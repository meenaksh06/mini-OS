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
    int sign = 1;

    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }

    while (str[i] != '\0') {
        num = num * 10 + (str[i] - '0');
        i++;
    }

    return num * sign;
}

char* int_to_str(int num) {
    char* str = (char*) alloc(20);

    int i = 0;
    int is_negative = 0;
    int temp = num;

    if (temp < 0) {
        is_negative = 1;
        temp = -temp;
    }

    if (temp == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    while (temp > 0) {
        str[i++] = (temp % 10) + '0';
        temp /= 10;
    }

    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

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
    n++; 

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

int str_distance(char* a, char* b) {
    int len_a = str_len(a);
    int len_b = str_len(b);
    
    int matrix[20][20];
    
    if (len_a >= 20 || len_b >= 20) return 999;

    for (int i = 0; i <= len_a; i++) matrix[i][0] = i;
    for (int j = 0; j <= len_b; j++) matrix[0][j] = j;

    for (int i = 1; i <= len_a; i++) {
        for (int j = 1; j <= len_b; j++) {
            int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
            
            int del = matrix[i - 1][j] + 1;
            int ins = matrix[i][j - 1] + 1;
            int sub = matrix[i - 1][j - 1] + cost;
            
            int min = del < ins ? del : ins;
            min = sub < min ? sub : min;
            
            matrix[i][j] = min;
        }
    }
    
    return matrix[len_a][len_b];
}

int str_contains(char* haystack, char* needle) {
    int h_len = str_len(haystack);
    int n_len = str_len(needle);
    
    if (n_len == 0) return 1;
    if (h_len < n_len) return 0;
    
    for (int i = 0; i <= h_len - n_len; i++) {
        int match = 1;
        for (int j = 0; j < n_len; j++) {
            if (haystack[i+j] != needle[j]) {
                match = 0;
                break;
            }
        }
        if (match) return 1;
    }
    return 0;
}

void str_to_lower(char* str) {
    int i = 0;
    while (str[i]) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + 32;
        }
        i++;
    }
}