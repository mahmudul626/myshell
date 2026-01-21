#include "../include/myshell.h"

int pid_function(char **args) {

    int status;
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    } 
    else if (pid == 0) {
        // Child Process
        if (execvp(args[0], args) == -1) {
            fprintf(stderr, "myShell: command not found: %s\n", args[0]);
            exit(EXIT_FAILURE); 
        }
    } 
    else {
        // Parent Process
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) == 0) {
                return 1;
            } else {
                return -1;
            }
        }
    }

    return -1;
    

}