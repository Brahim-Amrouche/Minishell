/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 12:57:10 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/25 19:45:38 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>

char	*terminate_curr__get_next_input(char *input)
{
	if (!input)
		return (NULL);
	while (*input && *input != '*')
		input++;
	if (*input == '*')
	{
		*(input++) = '\0';
		while (*input == '*')
			input++;
		if (*input)
			return (input);
		return (NULL);
	}
	return (NULL);
}

static int get_wildcard(char *str, char *in)
{
	int i;
	int len;
	char *match;
	char *next_in;
	char *input;

	if (!str || !in)
		return (0);
	input = ft_strdup(in);
	if (!ft_strncmp(str, "..", -1))
		return (0);
	if (*str == '.' && *in != '.')
		return (0);
	if (*input != '*' && *input != *str)
		return 0;
    if (input[ft_strlen(input) - 1] != '*'
        && input[ft_strlen(input) - 1] != str[ft_strlen(str) - 1])
        return 0;
	if (ft_strchr(input, '*') == NULL)
		return (0);
	// if (!*input)
	// 	return (0);
	while (*input == '*')
		input++;
	while (*str)
	{
		if (!*input)
			return (1);
		next_in = terminate_curr__get_next_input(input);
		match = ft_strnstr(str, input, -1);
		if (!match)
				return (0);
		if (!next_in)
			return (1);
		str = match + ft_strlen(input);
		input = next_in;
	}
	return (0);
}

int main(int argc, char *argv[], char **envp)
{
	DIR		*dir;
	struct	dirent *direntf;

	dir = opendir("/Users/maboulkh/Desktop/Minishell");
	while (1)
	{
		direntf = readdir(dir);
		if (!direntf)
			break;
		if (get_wildcard(direntf->d_name, argv[1]))
			printf("%s\n", direntf->d_name);
	}
}