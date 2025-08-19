#ifndef SHELL_H
#define SHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
extern char **environ;
void print_prompt(void);
char *read_command(void);
char *trim_whitespace(char *str);
char **parse_command(char *command);
void execute_command(char **args, char *prog_name);
char *find_command(char *command);
#endif
