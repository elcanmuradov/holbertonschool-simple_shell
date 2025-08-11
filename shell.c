#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

extern char **environ;

void display_prompt(void)
{
write(STDOUT_FILENO, "($) ", 4);
}

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

void execute_command(char *cmd, char *argv0)
{
pid_t pid;
int status;
char *argv[2];
argv[0] = cmd;
argv[1] = NULL;

pid = fork();
if (pid == -1)
{
perror("fork");
exit(EXIT_FAILURE);
}

if (pid == 0)
{
execve(cmd, argv, environ);
fprintf(stderr, "%s: 1: %s: not found\n", argv0, cmd);
exit(127);
}

do {
waitpid(pid, &status, WUNTRACED);
} while (!WIFEXITED(status) && !WIFSIGNALED(status));
}

int main(int argc, char *argv[])
{
char *command;
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

if (strcmp(command, "exit") == 0)
{
free(command);
break;
}

execute_command(command, argv[0]);
free(command);
}

return (0);
}
