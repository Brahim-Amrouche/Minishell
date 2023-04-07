/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:57:04 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/07 18:39:26 by maboulkh         ###   ########.fr       */
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

static int	echo(t_minishell *minishell, char *cmd)
{
	t_boolean new_line;

	new_line = TRUE;
	// psudo parsing -----------
	cmd = cmd + 5;
	if (!ft_strncmp("-n ", cmd, 3))
	{
		new_line = FALSE;
		cmd = cmd + 3;
	}
	// psudo parsing -----------
	if (!ft_strncmp("$?", cmd, 2))
		printf("%d", minishell->cmd_status);
	else
		printf("%s", cmd);
	if (new_line)
		printf("\n");
	return (0);
}

static int	change_dir(t_list *token)
{
	char	*path;

	path = NULL;
	if (token->next)
	path = token->next->content;
	if (!path)
		path = "/";
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
	chdir(path);
	free(path);
	return (0);
}

int	exec_cmd(t_minishell *minishell, char *cmd)
{
	t_list *token;

	token = minishell->tokens;
	// while (tokens)
    // {
        printf("tokens ==> |%s|\n", (char *)token->content);
    //     tokens = tokens->next;
    // }
	if (!cmd)
		exit_minishell(-1, NULL, TRUE);
	else if (!cmd)
		minishell->cmd_status = 0;
	else if (!ft_strncmp(CD, cmd, ft_strlen(CD)))
		minishell->cmd_status = change_dir(token);
	else if (!ft_strncmp(PWD, cmd, ft_strlen(PWD)))
		minishell->cmd_status = get_dir();
	else if (!ft_strncmp(ECHO, cmd, ft_strlen(ECHO)))
		minishell->cmd_status = echo(minishell, cmd);
	else if (!ft_strncmp(BASH_EXIT, cmd, ft_strlen(BASH_EXIT)))
		exit_minishell(-1, NULL, TRUE);
	return (minishell->cmd_status);
}
