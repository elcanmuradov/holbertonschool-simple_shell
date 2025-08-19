#include "shell.h"

int main(int argc, char **argv)
{
char *command;
char *prog_name = argv[0];
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

execute_command(command, prog_name);
free(command);
}

return 0;
}
