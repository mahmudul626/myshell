#include "../include/myshell.h"

int main() {
    char command[2014];
    char *args[200];
    int run = 1;

    while (run)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("\033[1;32mmyShell:\033[1;34m%s\033[0m$ ", cwd);
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



        if (strcmp(args[0], "exit") == 0) {
            run = 0;
            continue;
        } else if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL)
            {
                chdir(getenv("HOME"));
            } else {
                if(chdir(args[1]) != 0)
                        perror("cd");
            }
            continue;   
        } else if (strcmp(args[0], "pwd") == 0) {
            printf("%s\n", cwd);
            continue;
        } else if (strcmp(args[0], "help") == 0)
        {
            printf("Built-in commands:\n");
            printf("  cd    Change directory\n");
            printf("  pwd   Show current directory\n");
            printf("  exit  Exit shell\n");
            printf("  help  Show this help\n");
            continue;
        }
        
        pid_function(args);
        
    }
    return 0;
}

