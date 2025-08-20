#include "shell.h"

void execute_command(char **args, char *prog_name)
{
pid_t pid;
int status;
char *command_path;

command_path = find_command(args[0]);
if (!command_path)
{
fprintf(stderr, "%s: 1: %s: not found\n", prog_name, args[0]);
fflush(stderr);
exit(127);
}

pid = fork();
if (pid == -1)
{
perror("fork");
free(command_path);
return;
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
}
}

int main(int argc, char **argv)
{
char *command;
char *prog_name = argv[0];
char **args;

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

execute_command(args, prog_name);

free(args);
free(command);
}

return 0;
}
