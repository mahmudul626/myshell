#include "../include/myshell.h"

int execute_command(char **args, char *cwd) {

    int status = built_in(args, cwd);

        if (status == 0)
        {
            return status;
        } else if (status == 1)
        {
            return status;
        } else {

            int pid = pid_function(args);
            if (pid == 1)
            {
                return pid;
            } else {
                return pid;
            }
            

        }

}
