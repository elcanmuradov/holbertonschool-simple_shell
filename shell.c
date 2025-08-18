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

if (nread > 0 && line[nread - 1] == '\n')
line[nread - 1] = '\0';

return (line);
}

void execute_command(char *command, char *prog_name)
{
pid_t pid;
int status;
struct stat st;
char **argv;
char *command_copy;
char *token;
int i = 0;

trim_whitespace(command);

if (command[0] == '\0')
return;

command_copy = malloc(strlen(command) + 1);
if (command_copy == NULL)
return;
strcpy(command_copy, command);

argv = malloc(sizeof(char *) * 64);
if (argv == NULL)
{
free(command_copy);
return;
}

token = strtok(command_copy, " \t");
while (token != NULL && i < 63)
{
argv[i] = token;
token = strtok(NULL, " \t");
i++;
}
argv[i] = NULL;

if (argv[0] == NULL)
{
free(argv);
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
