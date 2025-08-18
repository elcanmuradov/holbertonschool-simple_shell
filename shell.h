#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

extern char **environ;

void print_prompt(void);
char *read_command(void);
void execute_command(char *command, char *prog_name);
char *trim_whitespace(char *str);


#endif /* SHELL_H */
