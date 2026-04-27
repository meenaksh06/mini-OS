#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "../memory/memory.h"
#include "../process/process.h"

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

/* Non-blocking readline: ticks the scheduler between keystrokes so
   background processes run while the shell is idle at the prompt. */
char* read_line_bg() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt          = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN]  = 0;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char* buf = (char*)alloc(100);
    int   i   = 0;
    char  c;

    while (1) {
        fflush(stdout);
        if (read(STDIN_FILENO, &c, 1) > 0) {
            if (c == '\n' || c == '\r') {
                write(STDOUT_FILENO, "\n", 1);
                break;
            }
            if ((c == 127 || c == 8) && i > 0) {   /* backspace */
                i--;
                write(STDOUT_FILENO, "\b \b", 3);
            } else if (c >= 32 && c <= 126 && i < 99) {
                buf[i++] = c;
                write(STDOUT_FILENO, &c, 1);
            }
        } else {
            scheduler_tick();
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    buf[i] = '\0';
    return buf;
}