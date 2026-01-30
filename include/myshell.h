#ifndef MYSHELL_H
#define MYSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <dirent.h>
#include <sys/wait.h>

int pid_function(char **args);
int built_in(char **args, char *cwd);
int execute_command(char **args, char *cwd);
int read_input(char *buffer, size_t size);

#endif