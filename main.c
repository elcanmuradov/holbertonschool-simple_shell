#include "shell.h"

int execute_command(char **args, char *prog_name)
{
pid_t pid;
int status;
char *command_path;

command_path = find_command(args[0]);
if (!command_path)
{
fprintf(stderr, "%s: 1: %s: not found\n", prog_name, args[0]);
fflush(stderr);
return 127;
}

pid = fork();
if (pid == -1)
{
perror("fork");
free(command_path);
return 1;
}

if (pid == 0)
{
execve(command_path, args, environ);
fprintf(stderr, "%s: 1: %s: not found\n", prog_name, args[0]);
fflush(stderr);
free(command_path);
_exit(127);
}
else
{
free(command_path);
do {
waitpid(pid, &status, WUNTRACED);
} while (!WIFEXITED(status) && !WIFSIGNALED(status));

if (WIFEXITED(status))
return WEXITSTATUS(status);
}
return 0;
}

int main(int argc, char **argv)
{
char *command;
char *prog_name = argv[0];
char **args;
int exit_status;
int i;

(void)argc;
signal(SIGINT, SIG_IGN);

while (1)
{
print_prompt();

command = read_command();
if (!command)
{
if (isatty(STDIN_FILENO))
printf("\n");
break;
}

trim_whitespace(command);

if (command[0] == '\0')
{
free(command);
continue;
}

args = parse_command(command);
if (!args)
{
free(command);
continue;
}

if (args[0] == NULL)
{
free(args);
free(command);
continue;
}

if (strcmp(args[0], "exit") == 0)
{
free(args);
free(command);
break;
}

if (strcmp(args[0], "env") == 0)
{
for (i = 0; environ[i]; i++)
{
printf("%s\n", environ[i]);
}
free(args);
free(command);
continue;
}

exit_status = execute_command(args, prog_name);

free(args);
free(command);

if (exit_status != 0)
exit(exit_status);
}

return 0;
}
