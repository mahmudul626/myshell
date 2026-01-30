#include "../include/myshell.h"

int read_input(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) return 0; // EOF
    buffer[strcspn(buffer, "\n")] = 0; // newline remove
    return 1;
}
