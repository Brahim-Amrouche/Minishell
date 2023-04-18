/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:01:08 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/18 07:59:57 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	change__update_pwds(t_minishell *minishell, char *path)
{
	char	*dir;
	t_list	token;

	dir = getcwd(NULL, 0);
	token.next = NULL;
	if (dir == NULL)
		exit_minishell(1, "cwd buffer is not enough", TRUE);
	token.content = pro_strjoin("OLDPWD=", dir);
	free(dir);
	if (chdir(path) != 0)
		return (1);
	export(minishell, &token);
	dir = getcwd(NULL, 0);
	if (dir == NULL)
		exit_minishell(1, "cwd buffer is not enough", TRUE);
	token.content = pro_strjoin("PWD=", dir);
	free(dir);
	export(minishell, &token);
	return (0);
}

int	change_dir(t_minishell *minishell, t_list *token)
{
	char	*path;
	int		status;
	char	**home_env;

	path = NULL;
	if (token->next)
		path = token->next->content;
	if (path && !*path)
	{
		home_env = get_env_var("HOME", minishell->envp);
		if (!home_env)
		{
			print_msg(2, "minishell: cd: HOME not set");
			return (1);
		}
		if ((*home_env)[5] == '\0')
			path = ft_strjoin("./", NULL);
		else
			path = ft_strdup((*home_env) + 5);
	}
	else if (path && *path != '/')
	path = ft_strjoin("./", path);
	else
		path = ft_strdup(path);
	if (access(path, F_OK) != 0)
		print_msg(2, "minishell: cd: $: No such file or directory", path);
	else if (access(path, X_OK) != 0)
		print_msg(2, "minishell: cd: $/: Permission denied", path);
	if (access(path, X_OK) != 0)
	{
		free(path);
		return (1);
	}
	status = change__update_pwds(minishell, path);
	free(path);
	return (status);
}
