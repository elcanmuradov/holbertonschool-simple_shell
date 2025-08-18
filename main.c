#include "shell.h"

/**
 * main - entry point for simple shell
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 on success
 */
int main(int argc, char **argv)
{
char *command;
char *prog_name = argv[0];

(void)argc;

while (1)
{
print_prompt();
command = read_command();

if (command == NULL)
{
if (isatty(STDIN_FILENO))
printf("\n");
break;
}
if (strlen(command) == 0)
{
free(command);
continue;
}

execute_command(command, prog_name);
free(command);
}

return (0);
}
