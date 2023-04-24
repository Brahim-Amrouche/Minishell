#include <dirent.h>
#include <stdio.h>
#include <string.h>


static int get_wildcard(char *str, char *input)
{
	int i;
	int len;
	int offset;

	if (!str || !input)
		return (0);
	i = 0;
	while (*(input + i) && *(str + i) == *(input + i) && *(input + i) != '*')
		i++;
	if (*(input + i) != '*')
		return (0);
	input++;
	len = strlen(str);
	if (i > strlen(str) - strlen(input + i))
		return (0);
	offset = len - strlen(input);
	while (*(input + i) && *(str + offset + i) == *(input + i))
		i++;
	if (offset + i != len)
		return (0);
	return (1);
}

int main(int argc, char *argv[], char **envp)
{
	DIR *dir;
	struct dirent *direntf;

	dir = opendir("/workspaces/Minishell");
	while (1)
	{
		direntf = readdir(dir);
		if (!direntf)
			break;
		if (get_wildcard(direntf->d_name, argv[1]))
			printf("%s\n", direntf->d_name);
	}
}