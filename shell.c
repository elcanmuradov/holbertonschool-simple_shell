#include "shell.h"

/**
 * shell_loop - Main loop of the shell
 *
 * Description: Continuously prompts for input and executes commands
 * until EOF (Ctrl+D) is encountered
 */
void shell_loop(void)
{
char *line;
char *command;
int len;
while (1)
{
if (isatty(STDIN_FILENO))
printf(PROMPT);
line = read_line();
if (line == NULL)
break;
len = strlen(line);
if (len > 0 && line[len - 1] == '\n')
line[len - 1] = '\0';
if (strlen(line) == 0)
{
free(line);
continue;
}
command = line;
execute_command(command);
free(line);
}
if (isatty(STDIN_FILENO))
printf("\n");
}

/**
 * read_line - Read a line of input from stdin
 *
 * Return: The line from stdin, or NULL on EOF
 */
char *read_line(void)
{
char *line = NULL;
size_t bufsize = 0;
ssize_t characters;
characters = getline(&line, &bufsize, stdin);
if (characters == -1)
{
if (line)
free(line);
return (NULL);
}
return (line);
}

/**
 * execute_command - Execute a single command
 * @command: The command to execute
 *
 * Return: Always 1 to continue execution
 */
int execute_command(char *command)
{
pid_t pid;
int status;
struct stat st;
char *args[2];
if (stat(command, &st) == -1 || access(command, X_OK) == -1)
{
print_error(command);
return (1);
}
args[0] = command;
args[1] = NULL;
pid = fork();
if (pid == 0)
{
if (execve(command, args, environ) == -1)
{
print_error(command);
exit(EXIT_FAILURE);
}
}
else if (pid < 0)
{
perror("fork");
}
else
{
do {
waitpid(pid, &status, WUNTRACED);
} while (!WIFEXITED(status) && !WIFSIGNALED(status));
}
return (1);
}

/**
 * print_error - Print error message for command not found
 * @command: The command that was not found
 */
void print_error(char *command)
{
fprintf(stderr, "./shell: %s: No such file or directory\n", command);
}
