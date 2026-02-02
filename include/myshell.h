#ifndef MYSHELL_H
#define MYSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <dirent.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <errno.h>

int pid_function(char **args);
int built_in(char **args, char *cwd);
int execute_command(char **args, char *cwd);
int read_input(char *buffer, size_t size);
void auto_complete(char *buffer, size_t *i, size_t size, int last_char);
void sigint_handler(int signo);


#endif