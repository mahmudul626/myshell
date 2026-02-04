#include "../include/myshell.h"


int main() {
    char command[2014];
    char *args[200];
    int run = 1;

    struct sigaction sa;
    sa.sa_handler = sigint_handler;  // function declared in myshell.h
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;                 // NOT SA_RESTART
    sigaction(SIGINT, &sa, NULL);



    while (run)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("\033[1;32mmyShell:\033[1;34m%s\033[0m$ ", cwd);
        fflush(stdout);

        if (!read_input(command, sizeof(command))) break;

        int i = 0;
        char *token = strtok(command, " \t\n");
        while (token != NULL)
        {
            args[i++] = token;
            token = strtok(NULL, " \t\n");
        }

        args[i] = NULL;



        int and_index = -1;
        for (int i = 0; args[i] != NULL; i++)
        {
            if (strcmp(args[i], "&&") == 0)
            {
                and_index = i;
                break;
            }
            
            
        }


        int or_index = -1;
        for (int i = 0; args[i] != NULL; i++)
        {
            if (strcmp(args[i], "||") == 0)
            {
                or_index = i;
                break;
            }
              
        }


        if (and_index != -1)    {

        char *left[100];
        char *right[100];

        // left part
        for (int i = 0; i < and_index; i++)
            left[i] = args[i];
        left[and_index] = NULL;

        // right part
        int j = 0;
        for (int i = and_index + 1; args[i] != NULL; i++)
            right[j++] = args[i];
        right[j] = NULL;

        int left_status = execute_command(left, cwd);
        if (left_status == 1) {
            int right_status = execute_command(right, cwd);
            if (right_status == 0) run = 0; 
        }
        
        } else if (or_index != -1) {

        char *left[100];
        char *right[100];

        // left part
        for (int i = 0; i < or_index; i++)
            left[i] = args[i];
        left[or_index] = NULL;

        // right part
        int j = 0;
        for (int i = or_index + 1; args[i] != NULL; i++)
            right[j++] = args[i];
        right[j] = NULL;

        int left_status = execute_command(left, cwd);
        if (left_status != 1) { // left failed
            execute_command(right, cwd); 
        }

        } else {

            int status = execute_command(args, cwd);
            if (status == 0) run = 0;

        }

        
    }
    save_history_to_file();
    return 0;
}

