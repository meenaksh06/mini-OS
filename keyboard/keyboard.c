#include <stdio.h>
#include "../memory/memory.h"

char* read_line() {
    char* buffer = (char*) alloc(100);
    int i = 0;
    char c;

    while ((c = getchar()) != '\n') {
        buffer[i++] = c;
    }

    buffer[i] = '\0';
    return buffer;
}