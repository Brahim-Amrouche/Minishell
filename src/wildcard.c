/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 12:57:10 by maboulkh          #+#    #+#             */
/*   Updated: 2023/05/29 21:32:37 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*terminate_curr__get_next_input(char *input)
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

static int check_wildcard(char *str, char *input)
{
	if (!str || !input)
		return (1);
	if (!ft_strncmp(str, "..", -1))
		return (1);
	if (*str == '.' && *input != '.')
		return (1);
	if (*input != '*' && *input != *str)
		return (1);
    if (input[ft_strlen(input) - 1] != '*'
		&& input[ft_strlen(input) - 1] != str[ft_strlen(str) - 1])
		return (1);
	if (!ft_strchr(input, '*'))
		return (1);
	return (0);
}

static int get_wildcard(char *str, char *in)
{
	char *match;
	char *next_in;
	char *input;

	if (check_wildcard(str, in))
		return (0);
		
	input = pro_str_dup(in);
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

char	**replace_wildcard(char *pattern, char **args)
{
	char	*new_elem;
	char	*cwd;
	DIR		*dir;
	struct	dirent *direntf;
	int		i;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		exit(1);
	dir = opendir(cwd);
	free(cwd);
	i = 0;
	while (TRUE)
	{
		direntf = readdir(dir);
		if (!direntf)
			break;
		if (get_wildcard(direntf->d_name, pattern))
		{
			new_elem = pro_str_dup(direntf->d_name);
			args = add_elem_to_arr(args, new_elem);
			// args = add_element_to_array(args, new_elem, sizeof(new_elem));
			i++;
		}
	}
	if (i == 0)
	{
		new_elem = pro_str_dup(pattern);
		args = add_elem_to_arr(args, new_elem);
		// args = add_element_to_array(args, new_elem, sizeof(new_elem));
	}
	closedir(dir);
	return (args);
}

// char	*make_wildcard_str(char *pattern)
// {
// 	char	*wildcard;
// 	char	*cwd;
// 	DIR		*dir;
// 	struct	dirent *direntf;

// 	wildcard = NULL;
// 	cwd = getcwd(NULL, 0);
// 	if (cwd == NULL)
// 		exit(1);
// 	// exit_minishell(1, "cwd error", TRUE);
// 	dir = opendir(cwd);
// 	free(cwd);
// 	while (TRUE)
// 	{
// 		direntf = readdir(dir);
// 		if (!direntf)
// 			break;
// 		if (get_wildcard(direntf->d_name, pattern))
// 		{
// 			if (wildcard)
// 				wildcard = pro_strjoin(wildcard, " ");
// 			wildcard = pro_strjoin(wildcard, direntf->d_name);
// 		}
// 	}
// 	// printf("|%s|\n", wildcard);
// 	closedir(dir);
// 	return (0);
// }
// execute wwith -> ./a.out 'wildcard'
// cc wildcard.c src/parsing/utils/strjoin.c libft/libft.a -I./libft/includes -I./includes -fsanitize=address -lreadline -lhistory -L/Users/maboulkh/.local/lib/readline/8.2.1/lib -DREADLINE_LIBRARY -I/Users/maboulkh/.local/lib/readline/8.2.1/include/readline
// int main(int argc, char *argv[], char **envp)
// {
// 	make_wildcard_str(argv[1]);
// }