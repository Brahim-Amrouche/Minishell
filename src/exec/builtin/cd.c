/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:01:08 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/03 18:50:28 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*save_oldpwd(t_minishell *minishell)
{
	char	*old_pwd;
	char	*env_pwd;
	char	*pwd_value;
	char	*temp;

	pwd_value = getcwd(NULL, 0);
	temp = pwd_value;
	if (!pwd_value)
		env_pwd = *get_env_var("PWD", minishell->envp);
	if (!pwd_value && !env_pwd)
	{
		free(temp);
		return (NULL);
	}
	if (!pwd_value)
		pwd_value = env_pwd + ft_strlen("PWD=");
	old_pwd = pro_strjoin("OLDPWD=", pwd_value);
	free(temp);
	return (old_pwd);
}

static char	*save_pwd(t_minishell *minishell, char *path)
{
	char	*pwd;
	char	*env_pwd;
	char	*pwd_value;
	char	*temp;

	pwd_value = getcwd(NULL, 0);
	temp = pwd_value;
	if (!pwd_value)
	{
		print_msg(2, "cd: error retrieving current directory: getcwd: $",
			"cannot access parent directories: No such file or directory");
		env_pwd = *get_env_var("PWD", minishell->envp);
		if (!env_pwd)
		{
			free(temp);
			return (NULL);
		}
		if (*path == '.')
			pwd_value = pro_strjoin(env_pwd + ft_strlen("PWD="), path + 1);
		else if (*path == '/')
			pwd_value = path;
	}
	pwd = pro_strjoin("PWD=", pwd_value);
	free(temp);
	return (pwd);
}

static int	change__update_pwds(t_minishell *minishell, char *path)
{
	char	*old_pwd;
	char	*pwd;

	old_pwd = save_oldpwd(minishell);
	if (chdir(path) != 0)
		return (1);
	pwd = save_pwd(minishell, path);
	mem_move(m_info(0, 1, pwd, ENV_SCOPE));
	mem_move(m_info(0, 1, old_pwd, ENV_SCOPE));
	mini_export(minishell, pwd);
	mini_export(minishell, old_pwd);
	return (0);
}

static char	*go_big_or_go_home(t_minishell *minishell, char *path, int *stat)
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

int	change_dir(t_minishell *minishell, char **args)
{
	char	*path;
	int		status;

	status = 0;
	path = NULL;
	if (*(args + 1))
		path = *(args + 1);
	if (!path)
		path = go_big_or_go_home(minishell, path, &status);
	else if (*path == '/')
		path = ft_strdup(path);
	else
		path = pro_strjoin("./", path);
	if (status)
		return (1);
	if (access(path, F_OK) == 0 && !is_directory(path))
		return (return_msg(1, "#cd: $: Not a directory", *(args + 1)));
	else if (access(path, F_OK) != 0)
		print_msg(2, "#cd: $: No such file or directory", *(args + 1));
	else if (access(path, X_OK) != 0)
		print_msg(2, "#cd: $/: Permission denied", *(args + 1));
	if (access(path, X_OK) != 0)
		return (1);
	status = change__update_pwds(minishell, path);
	return (status);
}
