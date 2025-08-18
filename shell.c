#include "shell.h"

/**
 * print_prompt - prints the shell prompt
 */
void print_prompt(void)
{
if (isatty(STDIN_FILENO))
printf("($) ");
}

/**
 * read_command - reads a command from stdin
 * Return: the command string or NULL on EOF
 */
char *read_command(void)
{
char *line = NULL;
size_t len = 0;
ssize_t nread;
nread = getline(&line, &len, stdin);
if (nread == -1)
{
free(line);
return (NULL);
}
if (line[nread - 1] == '\n')
line[nread - 1] = '\0';
return (line);
}

/**
 * execute_command - executes a command
 * @command: the command to execute
 * @prog_name: name of the shell program for error messages
 */
void execute_command(char *command, char *prog_name)
{
pid_t pid;
int status;
struct stat st;
if (command[0] == '\0')
return;
if (stat(command, &st) == -1)
{
fprintf(stderr, "%s: 1: %s: not found\n", prog_name, command);
return;
}
if (!(st.st_mode & S_IXUSR))
{
fprintf(stderr, "%s: 1: %s: Permission denied\n", prog_name, command);
return;
}
pid = fork();
if (pid == -1)
{
perror("fork");
return;
}

if (pid == 0)
{
char *argv[] = {command, NULL};
execve(command, argv, environ);
perror("execve");
_exit(127);
}
else
{
wait(&status);
}
}
