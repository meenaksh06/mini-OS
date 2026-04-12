#include <stdio.h>
#include "screen.h"

void clear_screen() {
    printf("\033[2J");
}

void move_cursor(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

void print_string(char* str) {
    printf("%s", str);
}