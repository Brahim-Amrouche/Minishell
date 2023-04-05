/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binaries.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 15:25:39 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/05 15:42:01 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"


static	t_boolean path_sep(char c)
{
	if (c == ';')
		return TRUE;
	return FALSE;
}

static void	add_ending_slash(char *paths[])
{
	size_t	path_len;
	char	*temp;

	while (*paths)
	{
		path_len = ft_strlen(*paths);
		if (*((*paths) + path_len - 1) != '/')
		{
			temp = *paths;
			*paths = pro_strjoin(*paths, "/");
			if (!*paths)
				exit_pipex(ENOMEM, "couldn't add slash's to path", TRUE);
			ft_free_node(1, temp);
		}
		paths++;
	}
}

static char	**parse_path(char *envp[])
{
	char	*res;
	char	**paths;

	res = find_env_var(envp, "PATH");
	if (!res)
		paths = ft_split_multi_sep("", path_sep);
	else
		paths = ft_split_multi_sep(res, path_sep);
	if (!paths)
		exit_pipex(ENOMEM, "couldn't malloc path routes", TRUE);
	add_ending_slash(paths);
	return (paths);
}


void	binary_parser(t_list *token_node, t_minishell *mini)
{
	char *token;

}

