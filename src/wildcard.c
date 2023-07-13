/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 12:57:10 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/13 01:49:52 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_wildcard(char *str, char *input)
{
	if (!str || !input)
		return (1);
	if (match_str(str, ".."))
		return (1);
	if (*str == '.' && *input != '.')
		return (1);
	if (!ft_strchr(input, '*'))
		return (1);
	return (0);
}

static int	get_wildcard(char *str, char *in)
{
	char	*match;
	char	**p_arr;
	int		i;

	p_arr = create_pattern_arr(in);
	if (!p_arr || check_wildcard(str, in))
		return (0);
	i = -1;
	while (p_arr[++i])
	{
		if (!(p_arr[i][0] == '\0'))
		{
			if (i == 0 && *p_arr[i] && *p_arr[i] != *str)
				return (0);
			if (!p_arr[i + 1] && *p_arr[i]
				&& p_arr[i][ft_strlen(p_arr[i]) - 1] != str[ft_strlen(str) - 1])
				return (0);
			match = ft_strnstr(str, p_arr[i], -1);
			if (!match)
				return (0);
			str = match + ft_strlen(p_arr[i]);
		}
	}
	return (1);
}

static char	**make_wildcard_arr(DIR *dir, char *pattern)
{
	struct dirent	*direntf;
	char			*new_elem;
	char			**args;
	int				i;

	args = NULL;
	i = 0;
	while (TRUE)
	{
		direntf = readdir(dir);
		if (!direntf)
			break ;
		if (get_wildcard(direntf->d_name, pattern))
		{
			new_elem = pro_str_dup(direntf->d_name);
			args = add_element_to_array(args, &new_elem, sizeof(new_elem));
			i++;
		}
	}
	if (i == 0)
	{
		new_elem = pro_str_dup(pattern);
		args = add_element_to_array(args, &new_elem, sizeof(new_elem));
	}
	return (args);
}

void	sort_wildcard(char **wildcard)
{
	char	*help;
	int		i;
	int		j;

	if (!wildcard)
		return ;
	i = 0;
	while (wildcard[i])
	{
		j = 0;
		while (wildcard[j] && wildcard[j + 1])
		{
			if (ft_strncmp(wildcard[j], wildcard[j + 1], -1) > 0)
			{
				help = wildcard[j];
				wildcard[j] = wildcard[j + 1];
				wildcard[j + 1] = help;
			}
			j++;
		}
		i++;
	}
	return ;
}

char	**create_wildcard_arr(char *pattern)
{
	char	**args;
	char	*cwd;
	DIR		*dir;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (add_element_to_array(NULL, &pattern, sizeof(char *)));
	dir = opendir(cwd);
	free(cwd);
	args = make_wildcard_arr(dir, pattern);
	sort_wildcard(args);
	closedir(dir);
	return (args);
}
