#include "shell.h"
extern char **environ;

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

