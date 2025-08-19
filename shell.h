#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>

void print_prompt(void);
char *read_command(void);
void execute_command(char *command, char *prog_name);

#endif
