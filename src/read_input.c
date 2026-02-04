#include "../include/myshell.h"
#define PATH_MAX 4096
#define MAX_HIST 100
static char *history[MAX_HIST];
static int h_count = 0;
static int current_hist_idx = -1;

volatile sig_atomic_t sigint_received = 0;

void sigint_handler(int signo)
{
    (void)signo;
    sigint_received = 1;
    write(STDOUT_FILENO, "\n", 1);
}


void save_history_to_file() {
    char path[PATH_MAX];
    char *home = getenv("HOME");
    if (home == NULL) {
        printf("Failed to get path\n");
        return;
    }
    
    snprintf(path, sizeof(path), "%s/.history", home);

    FILE *fp = fopen(path, "a");
    if (fp == NULL) return;

    for (int j = 0; j < h_count; j++) {
        fprintf(fp, "%s\n", history[j]);
        free(history[j]); // Memory clean 
    }
    fclose(fp);
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
            current_hist_idx = -1;
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

        // arrow operations
        if (c == 27)
        {
            char seq[2];
            if(read(STDIN_FILENO, &seq[0], 1) != 1) continue;
            if(read(STDIN_FILENO, &seq[1], 1) != 1) continue;

            if (seq[0] == '[')
            {
                switch (seq[1])
                {
                case 'A': // Up Arrow
                    if (h_count > 0 && current_hist_idx < h_count - 1) {
                        while (i > 0) {
                            write(STDOUT_FILENO, "\b \b", 3);
                            i--;
                        }

                        current_hist_idx++;
                        char *prev_cmd = history[h_count - 1 - current_hist_idx];
                        strcpy(buffer, prev_cmd);
                        i = strlen(buffer);
                        write(STDOUT_FILENO, buffer, i);
                    }
                    break;

                case 'B': // Down Arrow
                    if (current_hist_idx >= 0) {
                        while (i > 0) {
                            write(STDOUT_FILENO, "\b \b", 3);
                            i--;
                        }

                        current_hist_idx--;
                        if (current_hist_idx == -1) {
                            buffer[0] = '\0';
                            i = 0;
                        } else {
                            char *next_cmd = history[h_count - 1 - current_hist_idx];
                            strcpy(buffer, next_cmd);
                            i = strlen(buffer);
                            write(STDOUT_FILENO, buffer, i);
                        }
                    }
                    break;

                case 'C':
                    break;

                case 'D':
                    break;
                }
                
            }
            

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

    if (i > 0) {
        if (h_count > 0 && strcmp(history[h_count-1], buffer) == 0) {
            //do noting
        }else {
            if (h_count < MAX_HIST) {
                history[h_count++] = strdup(buffer);
            } else {
                free(history[0]);
                for(int j = 1; j < MAX_HIST; j++) history[j-1] = history[j];
                history[MAX_HIST-1] = strdup(buffer);
            }
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 1;
}
