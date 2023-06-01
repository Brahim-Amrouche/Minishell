/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:01:08 by maboulkh          #+#    #+#             */
/*   Updated: 2023/06/01 16:28:33 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	change__update_pwds(t_minishell *minishell, char *path)
{
	char		*dir;
	char		*cmd[2];

	cmd[1] = NULL;
	dir = getcwd(NULL, 0);
	if (dir == NULL)
		exit_minishell(1, "cwd buffer is not enough", TRUE);
	cmd[0] = pro_strjoin("OLDPWD=", dir);
	free(dir);
	if (chdir(path) != 0)
		return (1);
	export(minishell, cmd, 0);
	dir = getcwd(NULL, 0);
	if (dir == NULL)
		exit_minishell(1, "cwd buffer is not enough", TRUE);
	cmd[0] = pro_strjoin("PWD=", dir);
	free(dir);
	export(minishell, cmd, 0);
	return (0);
}


static char	*go_to_weird_paths(t_minishell *minishell, char *path, int *stat)
{
	char	*env_path;
	char	**home_env;

	if (!path)
		return (0);
	env_path = NULL;
	if (!*path)
		env_path = "HOME";
	else if (*path == '-')
		env_path = "OLDPWD";
	home_env = get_env_var(env_path, minishell->envp);
	if (!home_env)
	{
		print_msg(2, "minishell: cd: $ not set", env_path);
		*stat = 1;
		return (NULL);
	}
	if ((*home_env)[ft_strlen(env_path) + 1] == '\0')
		path = pro_strjoin("./", NULL);
	else
		path = ft_strdup((*home_env) + ft_strlen(env_path) + 1);
	if (!ft_strncmp(env_path, "OLDPWD", 7) && home_env)
		print_msg(1, "$", (*home_env) + ft_strlen(env_path) + 1);
	return (path);
}

int	change_dir(t_minishell *minishell, char **args)
{
	char	*path;
	int		status;

	status = 0;
	path = NULL;
	if (*(args + 1))
		path = *(args + 1);
	if (path && (*path == '\0' || *path == '-'))
		path = go_to_weird_paths(minishell, path, &status);
	else if (path && *path != '/')
		path = pro_strjoin("./", path);
	else
		path = ft_strdup(path);
	if (status)
		return (1);
	else if (access(path, F_OK) != 0) // is access protected against NULL?
		print_msg(2, "minishell: cd: $: No such file or directory", path);
	else if (access(path, X_OK) != 0)
		print_msg(2, "minishell: cd: $/: Permission denied", path);
	if (access(path, X_OK) != 0)
	{
		// free(path);
		return (1);
	}
	status = change__update_pwds(minishell, path);
	// free(path);
	return (status);
}
