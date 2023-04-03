/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:57:04 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/03 05:42:57 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_dir(void)
{
	char	cwd[1024];

	if (getcwd(cwd, 1024) == NULL)
	{
		exit_minishell(1, "cwd buffer is not enough", TRUE);
	}
	else
		printf("%s\n", cwd);
	return (0);
}

static int	change_dir(char *cmd)
{
	char	*path;

	path = NULL;
	// psudo parsing -----------
	cmd = cmd + 3;
	// psudo parsing -----------
	if (*cmd != '/')
		path = ft_strjoin("./", cmd);
	else
		path = ft_strdup(cmd);
	if (access(path, X_OK) != 0)
	{
		print_msg(2, "minishell: cd: $/: Permission denied", cmd);
		return (1);
	}
	chdir(path);
	free(path);
	return (0);
}

int	exec_cmd(char *cmd)
{
	if (!ft_strncmp("cd ", cmd, 3))
		change_dir(cmd);
	else if (!ft_strncmp("pwd", cmd, 3))
		get_dir();
	return (0);
}
