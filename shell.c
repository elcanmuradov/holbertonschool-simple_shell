#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_CMD_LEN 1024

extern char **environ;

/**
 * display_prompt - prints the shell prompt
 */
void display_prompt(void)
{
write(STDOUT_FILENO, "($) ", 4);
}

/**
 * read_command - reads a line from stdin
 * Return: pointer to command string, or NULL on EOF
 */
char *read_command(void)
{
char *cmd = NULL;
size_t len = 0;
ssize_t n;
n = getline(&cmd, &len, stdin);
if (n == -1)
{
free(cmd);
return (NULL);
}
if (n > 0 && cmd[n - 1] == '\n')
cmd[n - 1] = '\0';
return (cmd);
}

/**
 * execute_command - runs a command using fork and execve
 * @cmd: command to run (full path)
 * @argv0: program name for error messages
 */
void execute_command(char *cmd, char *argv0)
{
pid_t pid;
int status;
pid = fork();
if (pid == -1)
{
perror("fork");
exit(EXIT_FAILURE);
}
if (pid == 0)
{
if (execve(cmd, &cmd, environ) == -1)
{
fprintf(stderr, "%s: 1: %s: not found\n", argv0, cmd);
exit(127);
}
}
else
{
do {
waitpid(pid, &status, WUNTRACED);
} while (!WIFEXITED(status) && !WIFSIGNALED(status));
}
}
/**
 * main - entry point
 * @argc: argument count
 * @argv: argument vector
 * Return: always 0
 */
int main(int argc, char *argv[])
{
char *command;
const char *exit_cmd = "exit";
int interactive = isatty(STDIN_FILENO);
(void)argc;
signal(SIGINT, SIG_IGN);
while (1)
{
if (interactive)
{
display_prompt();
fflush(stdout);
}
command = read_command();
if (!command)
{
if (interactive)
write(STDOUT_FILENO, "\n", 1);
break;
}
if (strcmp(command, exit_cmd) == 0)
{
free(command);
break;
}
if (access(command, X_OK) == 0)
{
execute_command(command, argv[0]);
}
else
{
fprintf(stderr, "%s: 1: %s: not found\n", argv[0], command);
}
free(command);
}
return (0);
}
