// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>
// #include <termios.h>
// #include <unistd.h>
// #include <dirent.h>
#include "../include/myshell.h"

int read_input(char *buffer, size_t size) {
    struct termios oldt, newt;
    int c, last_char = 0;
    size_t i = 0;

    // Terminal Mode change (Raw Mode)
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (1) {
        c = getchar();

        // EOF (Ctrl+D)
        if (c == EOF) {
            if (i == 0) {
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                return 0;
            }
            break;
        }

        // Backspace handle
        if (c == 127 || c == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b");
                fflush(stdout);
            }
            last_char = c;
            continue;
        }

        //  Tab handle
        if (c == '\t') {
            buffer[i] = '\0';
            auto_complete(buffer, &i, size, last_char);
            last_char = c;
            continue;
        }

        //Enter Handle
        if (c == '\n') {
            putchar('\n');
            break;
        }

        // General  char handle
        if (isprint(c)) {
            if (i < size - 1) {
                buffer[i++] = (char)c;
                putchar(c);
                fflush(stdout);
            }
        }
        last_char = c;
    }

    buffer[i] = '\0';
    
    // Back terminal to prev mode
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 1;
}