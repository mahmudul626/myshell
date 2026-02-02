#include "../include/myshell.h"


volatile sig_atomic_t sigint_received = 0;

void sigint_handler(int signo)
{
    (void)signo;
    sigint_received = 1;
    write(STDOUT_FILENO, "\n", 1);
}

int read_input(char *buffer, size_t size)
{
    struct termios oldt, newt;
    char c;
    size_t i = 0;
    ssize_t n;
    int last_char = 0;

    // Save terminal state
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (1) {

        n = read(STDIN_FILENO, &c, 1);

        // read() error
        if (n < 0) {
            if (errno == EINTR) {
                if (sigint_received) {
                    sigint_received = 0;
                    buffer[0] = '\0';
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                    return 1;   // Ctrl+C → empty command, new prompt
                }
                continue;
            }
            break; // real error
        }

        // EOF (Ctrl+D)
        if (c == 4) {
            if (i == 0) {
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                return 0;       // exit shell
            }
            break;              // accept typed line
        }

        //Enter
        if (c == '\n') {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        //Backspace
        if (c == 127 || c == '\b') {
            if (i > 0) {
                i--;
                write(STDOUT_FILENO, "\b \b", 3);
            }
            last_char = c;
            continue;
        }

        //Tab (autocomplete)
        if (c == '\t') {
            buffer[i] = '\0';
            auto_complete(buffer, &i, size, last_char);
            last_char = c;
            continue;
        }

        //Printable char 
        if (isprint((unsigned char)c) && i < size - 1) {
            buffer[i++] = c;
            write(STDOUT_FILENO, &c, 1);
        }

        last_char = c;
    }

    buffer[i] = '\0';
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 1;
}
