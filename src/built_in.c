#include "../include/myshell.h"

int built_in(char **args, char *cwd) {

    if (args[0] == NULL) return 1;



        if (strcmp(args[0], "exit") == 0) {
            return 0;
        } else if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL)
            {
                chdir(getenv("HOME"));
            } else {
                if(chdir(args[1]) != 0)
                        perror("cd");
            }
            return 1;   
        } else if (strcmp(args[0], "pwd") == 0) {
            printf("%s\n", cwd);
            return 1;
        } else if (strcmp(args[0], "history") == 0) {
            char *home = getenv("HOME");
            if (home == NULL) {
                perror("path");
                return 1;
            }

            // runtime PATH_MAX determination
            long path_max = pathconf(home, _PC_PATH_MAX);
            if (path_max == -1) path_max = 4096; // fallback

            char path[path_max + 20]; // ".history" + null terminator-এর জন্য extra space
            snprintf(path, sizeof(path), "%s/.history", home);

            FILE *fptr = fopen(path, "r");
            if (fptr == NULL) {
                perror("history");
                return 1;
            }

            char buffer[256];
            int line_num = 1;

            while (fgets(buffer, sizeof(buffer), fptr) != NULL) {
                printf(" %d  %s", line_num++, buffer);
            }

            fclose(fptr);
            return 1;
        } else if (strcmp(args[0], "help") == 0)
        {
            printf("Built-in commands:\n");
            printf("  cd        Change directory\n");
            printf("  pwd       Show current directory\n");
            printf("  exit      Exit shell\n");
            printf("  history   Display the command history list\n");
            printf("  help      Show this help\n");
            return 1;
        }

        return -1;
}