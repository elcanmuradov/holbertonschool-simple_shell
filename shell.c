#include "shell.h"

void print_prompt(void)
{
if (isatty(STDIN_FILENO))
{
printf("($) ");
fflush(stdout);
}
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

char **parse_command(char *command)
{
int bufsize = 64;
char **args = malloc(bufsize * sizeof(char*));
char *token;
int position = 0;
char **temp;

if (!args)
return NULL;

token = strtok(command, " \t\r\n");
while (token != NULL)
{
args[position] = token;
position++;

if (position >= bufsize)
{
bufsize += 64;
temp = realloc(args, bufsize * sizeof(char*));
if (!temp)
{
free(args);
return NULL;
}
args = temp;
}

token = strtok(NULL, " \t\r\n");
}
args[position] = NULL;

return args;
}

char *find_command(char *command)
{
char *path_env;
char *path_copy;
char *dir;
char *full_path;
struct stat st;
int path_len;
int cmd_len;

if (strchr(command, '/') != NULL)
{
if (stat(command, &st) == 0)
return strdup(command);
return NULL;
}

path_env = getenv("PATH");
if (!path_env)
return NULL;

path_copy = strdup(path_env);
if (!path_copy)
return NULL;

cmd_len = strlen(command);
dir = strtok(path_copy, ":");

while (dir != NULL)
{
path_len = strlen(dir);
full_path = malloc(path_len + cmd_len + 2);
if (!full_path)
{
free(path_copy);
return NULL;
}

strcpy(full_path, dir);
strcat(full_path, "/");
strcat(full_path, command);

if (stat(full_path, &st) == 0)
{
free(path_copy);
return full_path;
}

free(full_path);
dir = strtok(NULL, ":");
}

free(path_copy);
return NULL;
}

