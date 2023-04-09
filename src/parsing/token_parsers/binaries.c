/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binaries.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 15:25:39 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/09 00:05:56 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static char	*cmd_is_exect(char *cmd, char **paths)
{
	char		*joined_path;
	t_boolean	is_a_path;

	if (!access(cmd, X_OK))
		return cmd;
	is_a_path = str_is_a_path(cmd);
	if (is_a_path)
		return cmd;
	while (*paths)
	{
		joined_path = pro_strjoin(*paths, cmd);
		if (!joined_path)
			exit_minishell(ENOMEM, "could't join binary to paths", TRUE);
		if (!access(joined_path, X_OK))
		{
			ft_free_node(1, cmd);
			return joined_path;
		}
		ft_free_node(STRJOIN_SCOPE, joined_path);
		paths++;
	}
	return NULL;
}

static	t_boolean path_sep(char c)
{
	if (c == ':')
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
				exit_minishell(ENOMEM, "couldn't add slash's to path", TRUE);
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
		exit_minishell(ENOMEM, "couldn't malloc path routes", TRUE);
	add_ending_slash(paths);
	return (paths);
}



void	binary_parser(t_list *token_node, t_minishell *mini)
{
	char *token;
	char **paths;

	token = token_node->content;
	paths = parse_path(mini->envp);
	if (cmd_is_builtin(token))
		return ;
	token_node->content = cmd_is_exect(token, paths);
	if (!token_node->content)
		print_msg(2, "this is not an error just that you gave a wrong binary path");
		// exit_minishell(-1, "this is not an error just that you gave a wrong binary path", TRUE);
}

