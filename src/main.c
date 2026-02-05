#include "../include/myshell.h"


void sigchld_handler(int sig) {
    // মৃত প্রসেসগুলোকে পরিষ্কার (reap) করা
    // WNOHANG নিশ্চিত করে যে কোনো প্রসেস রানিং থাকলে শেল আটকে যাবে না
    while (waitpid(-1, NULL, WNOHANG) > 0);
}


int main() {
    char command[2014];
    char *args[200];
    int run = 1;

    struct sigaction sa;
    sa.sa_handler = sigint_handler;  // function declared in myshell.h
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;                 // NOT SA_RESTART
    sigaction(SIGINT, &sa, NULL);


    struct sigaction sa_chld;
    sa_chld.sa_handler = sigchld_handler;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART | SA_NOCLDSTOP; // SA_RESTART দিলে সিস্টেম কল ইন্টারাপ্ট হবে না
    if (sigaction(SIGCHLD, &sa_chld, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }



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


        int is_background = 0;
        // Check if the last argument is '&'
        if (i > 0 && strcmp(args[i-1], "&") == 0) {
            is_background = 1;
            args[i-1] = NULL; // Remove '&' so the command doesn't get confused
        }


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

        int left_status = execute_command(left, cwd, is_background);
        if (left_status == 1) {
            int right_status = execute_command(right, cwd, is_background);
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

        int left_status = execute_command(left, cwd, is_background);
        if (left_status != 1) { // left failed
            execute_command(right, cwd, is_background); 
        }

        } else {

            int status = execute_command(args, cwd, is_background);
            if (status == 0) run = 0;

        }

        
    }
    save_history_to_file();
    return 0;
}

