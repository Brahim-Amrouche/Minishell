/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:01:08 by maboulkh          #+#    #+#             */
/*   Updated: 2023/06/23 20:00:53 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *remove_dotdot(char *path)
{
	int		dot_dot;
	int		len;

	dot_dot = 0;
	len = ft_strlen(path);
	while (len > 3 && path[len - 1] == '.' && path[len - 2] == '.' && path[len - 3] == '/')
	{
		len -= 3;
		dot_dot++;
	}
	while (dot_dot && path[len])
	{
		if (path[len] != '/')
			dot_dot--;
		len--;
	}
	return (protected_substr(path, 0, len));
}

static int	change_dir_on_error(t_minishell *minishell, char *path)
{
	char	*cmd[3];
	char	*env_pwd;
	char	*new_pwd;
	char	*clean_path;

	cmd[0] = "export";
	cmd[2] = NULL;
	env_pwd = *get_env_var("PWD", minishell->envp);
	if (!env_pwd)
		return (return_msg(1, "can 't find current working dir"));
	cmd[1] = pro_strjoin("OLDPWD=", env_pwd + ft_strlen("PWD="));
	export(minishell, cmd, 0);
	if (*path == '.')
		path++;
	new_pwd = pro_strjoin(env_pwd, path);
	cmd[1] = pro_strjoin("PWD=", new_pwd);
	export(minishell, cmd, 0);
	clean_path = remove_dotdot(path);
	if (chdir(clean_path) != 0)
		return_msg(1, "cd: error retrieving current directory");
	ft_free_node(1, clean_path);
	return (0);
}

static int	change__update_pwds(t_minishell *minishell, char *path)
{
	char	*dir;
	char	*cmd[3];

	cmd[0] = "export";
	cmd[2] = NULL;
	dir = getcwd(NULL, 0);
	if (dir == NULL)
		return (change_dir_on_error(minishell, path));
	cmd[1] = pro_strjoin("OLDPWD=", dir);
	free(dir);
	if (chdir(path) != 0)
		return (1);
	export(minishell, cmd, 0);
	dir = getcwd(NULL, 0);
	if (dir == NULL)
		return (return_msg(1, "cd: error retrieving next directory"));
	cmd[1] = pro_strjoin("PWD=", dir);
	free(dir);
	export(minishell, cmd, 0);
	return (0);
}

static char	*go_to_weird_paths(t_minishell *minishell, char *path, int *stat)
{
	char	*env_path;
	char	**home_env;

	env_path = "HOME";
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
	return (path);
}

static t_boolean is_directory(char *path)
{
	struct stat	file_stat;

	if (stat(path, &file_stat) == 0)
	{
		if ((((file_stat.st_mode) & S_IFMT) == S_IFDIR))
			return (TRUE);
		else
			return (FALSE);
	}
	print_msg(2, "couldn' t retrive file stat");
	return (FALSE);
}

int	change_dir(t_minishell *minishell, char **args)
{
	char	*path;
	int		status;

	status = 0;
	path = NULL;
	if (*(args + 1))
		path = *(args + 1);
	if (!path)
		path = go_to_weird_paths(minishell, path, &status);
	else if (*path == '/')
		path = ft_strdup(path);
	else
		path = pro_strjoin("./", path);
	if (status)
		return (1);
	if (access(path, F_OK) == 0 && !is_directory(path))
		return(return_msg(1, "minishell: cd: $: Not a directory", *(args + 1)));
	else if (access(path, F_OK) != 0)
		print_msg(2, "minishell: cd: $: No such file or directory", *(args + 1));
	else if (access(path, X_OK) != 0)
		print_msg(2, "minishell: cd: $/: Permission denied", *(args + 1));
	if (access(path, X_OK) != 0)
		return (1);
	status = change__update_pwds(minishell, path);
	return (status);
}
