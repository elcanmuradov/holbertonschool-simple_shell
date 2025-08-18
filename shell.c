#include "shell.h"

/**
 * print_prompt - prints the shell prompt
 */
void print_prompt(void)
{
if (isatty(STDIN_FILENO))
printf("($) ");
/**
 * trim_whitespace - removes leading and trailing whitespace from string
 * @str: string to trim
 * Return: pointer to trimmed string (modifies original)
 */
char *trim_whitespace(char *str)
{
char *start = str;
char *end;

/* Skip leading whitespace */
while (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r')
start++;

/* All spaces? */
if (*start == '\0')
{
*str = '\0';
return (str);
}

/* Trim trailing space */
end = start + strlen(start) - 1;
while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r'))
end--;

/* Write new null terminator */
*(end + 1) = '\0';

/* Move trimmed content to beginning if necessary */
if (start != str)
{
memmove(str, start, strlen(start) + 1);
}

return (str);
}

/**
 * parse_command - parses command line into arguments
 * @command: command line to parse
 * Return: array of arguments (NULL terminated)
 */
char **parse_command(char *command)
{
char **argv;
char *token;
int argc = 0;
int i;

/* Count arguments */
token = strtok(command, " \t");
while (token != NULL)
{
argc++;
token = strtok(NULL, " \t");
}

/* Allocate memory for argv array */
argv = malloc(sizeof(char *) * (argc + 1));
if (argv == NULL)
return (NULL);

/* Reset command string and parse again */
token = strtok(command, " \t");
for (i = 0; i < argc; i++)
{
argv[i] = token;
token = strtok(NULL, " \t");
}
argv[argc] = NULL;

return (argv);
}
/*
 * @str: string to trim
 * Return: pointer to trimmed string (modifies original)
*/
char *trim_whitespace(char *str)
{
char *start = str;
char *end;

while (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r')
start++;
if (*start == '\0')
{
*str = '\0';
return (str);
}
end = start + strlen(start) - 1;
while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r'))
end--;
*(end + 1) = '\0';
if (start != str)
{
memmove(str, start, strlen(start) + 1);
}

return (str);
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
char **argv;
char *command_copy;
trim_whitespace(command);
if (command[0] == '\0')
return;
command_copy = malloc(strlen(command) + 1);
if (command_copy == NULL)
return;
strcpy(command_copy, command);
argv = parse_command(command_copy);
if (argv == NULL)
{
free(command_copy);
return;
}
if (stat(argv[0], &st) == -1)
{
fprintf(stderr, "%s: 1: %s: not found\n", prog_name, argv[0]);
free(argv);
free(command_copy);
return;
}

if (!(st.st_mode & S_IXUSR))
{
fprintf(stderr, "%s: 1: %s: Permission denied\n", prog_name, argv[0]);
free(argv);
free(command_copy);
return;
}

pid = fork();
if (pid == -1)
{
perror("fork");
free(argv);
free(command_copy);
return;
}

if (pid == 0)
{
execve(argv[0], argv, environ);
perror("execve");
_exit(127);
}
else
{
wait(&status);
}

free(argv);
free(command_copy);
}
