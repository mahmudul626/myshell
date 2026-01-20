#include "../include/myshell.h"

void pid_function(char **args) {
        pid_t pid = fork();

                if (pid == 0)
                {
                    execvp(args[0], args);
                    fprintf(stderr, "myShell: command not found: %s\n", args[0]);
                    exit(1);
                } else {
                    wait(NULL);
                }

}