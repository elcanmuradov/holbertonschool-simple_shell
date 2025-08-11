#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

#define PROMPT "#cisfun$ "
#define MAX_LINE 1024

extern char **environ;

void shell_loop(void);
char *read_line(void);
int execute_command(char *command);
void print_error(char *command);

#endif /* SHELL_H */
