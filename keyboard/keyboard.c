#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "../memory/memory.h"

int key_pressed() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

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