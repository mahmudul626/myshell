#include "../include/myshell.h"


int main() {
    char command[2014];
    char *args[200];
    int run = 1;

    while (run)
    {
        printf("myShell> ");
        fflush(stdout);

        if (fgets(command, sizeof(command), stdin) == NULL) break; 
        command[strcspn(command, "\n")] = 0;

        int i = 0;
        char *token = strtok(command, " \t\n");
        while (token != NULL)
        {
            args[i++] = token;
            token = strtok(NULL, "\n");
        }

        args[i] = NULL;

        if (args[0] == NULL) continue;



        if (strcmp(args[0], "exit") == 0)
        {
            run = 0;
            continue;
        } else if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            {
                chdir(getenv("HOME"));
            } else {
                chdir(args[1]);
            }
            continue;
            
        }
        


        
        pid_t pid = fork();

        if (pid == 0)
        {
            execvp(args[0], args);
            perror("myShell");
            exit(1);
        } else {
            wait(NULL);
        }
        
        
        
    }
    
    
    return 0;
}
