#include "shell.h"

int main(int argc, char **argv)
{
char *command;
char *prog_name = argv[0];
char *token;
char **args;
int i, bufsize = 64;
(void)argc;

signal(SIGINT, SIG_IGN);

while (1)
{
if (isatty(STDIN_FILENO))
printf("($) ");
fflush(stdout);

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

args = malloc(bufsize * sizeof(char*));
if (!args)
{
free(command);
continue;
}

i = 0;
token = strtok(command, " \t\r\n");
while (token != NULL)
{
args[i] = token;
i++;

if (i >= bufsize)
{
bufsize += 64;
args = realloc(args, bufsize * sizeof(char*));
if (!args)
{
free(command);
break;
}
}
}
args[i] = NULL;

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
