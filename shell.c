#include "shell.h"

void print_prompt(void)
{
if (isatty(STDIN_FILENO))
printf("($) ");
fflush(stdout);
}

char *trim_whitespace(char *str)
{
char *start = str;
char *end;

while (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r')
start++;

if (*start == '\0')
{
*str = '\0';
return str;
}

end = start + strlen(start) - 1;
while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r'))
end--;

*(end + 1) = '\0';

if (start != str)
memmove(str, start, strlen(start) + 1);

return str;
}

char *read_command(void)
{
char *line = NULL;
size_t len = 0;
ssize_t nread;

nread = getline(&line, &len, stdin);
if (nread == -1)
{
free(line);
return NULL;
}

if (nread > 0 && line[nread - 1] == '\n')
line[nread - 1] = '\0';

return line;
}

char **split_line(char *line)
{
char **tokens;
char *token;
int bufsize = 64, i = 0;

tokens = malloc(bufsize * sizeof(char*));
if (!tokens)
{
perror("hsh");
exit(EXIT_FAILURE);
}

token = strtok(line, " \t\r\n");
while (token != NULL)
{
tokens[i] = token;
i++;

if (i >= bufsize)
{
bufsize += 64;
tokens = realloc(tokens, bufsize * sizeof(char*));
if (!tokens)
{
perror("hsh");
exit(EXIT_FAILURE);
}
}
}
tokens[i] = NULL;
return tokens;
}

void execute_command(char *command, char *prog_name)
{
pid_t pid;
int status;
char **args;
char *command_copy;

trim_whitespace(command);
if (command[0] == '\0')
return;

command_copy = strdup(command);
if (!command_copy)
return;

args = split_line(command_copy);

if (args[0] == NULL)
{
free(args);
free(command_copy);
return;
}

if (access(args[0], X_OK) != 0)
{
fprintf(stderr, "%s: 1: %s: not found\n", prog_name, args[0]);
free(args);
free(command_copy);
return;
}

pid = fork();
if (pid == -1)
{
perror("fork");
free(args);
free(command_copy);
return;
}

if (pid == 0)
{
execve(args[0], args, environ);
fprintf(stderr, "%s: 1: %s: not found\n", prog_name, args[0]);
free(args);
free(command_copy);
_exit(127);
}
else
{
do {
waitpid(pid, &status, WUNTRACED);
} while (!WIFEXITED(status) && !WIFSIGNALED(status));
}

free(args);
free(command_copy);
}
