/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 15:32:24 by maboulkh          #+#    #+#             */
/*   Updated: 2023/12/27 15:48:28 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	lunch_bin(char **args, t_minishell *mini)
{
	int		id;

	id = fork();
	if (id == -1)
		exit_minishell(1, "could't fork", TRUE);
	if (id == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		while (match_str(args[0], ""))
			args++;
		if (!*args)
			exit(0);
		binary_parser(args, mini);
		if (access(args[0], F_OK) || !str_is_a_path(args[0]))
			exit(return_msg(127, "minishell: $: command not found", args[0]));
		else if (access(args[0], X_OK))
			exit(return_msg(127, "minishell: $: permission denied", args[0]));
		else
			execve(args[0], args, mini->envp);
		if (args && args[0] && str_is_a_path(args[0]))
			exit(return_msg(126, "minishell: $: is a directory", args[0]));
		exit(return_msg(126, "minishell: $: command not found", args[0]));
	}
	return (id);
}

static void	lunch_bin_wraper(char **args, t_minishell *mini, int *status)
{
	int		id;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	id = lunch_bin(args, mini);
	g_sigvar.in_child = TRUE;
	wait_all(id, status);
	g_sigvar.in_child = FALSE;
}

static void	update_underscore(t_minishell *minishell, char **args)
{
	int		i;
	char	*underscore;

	if (!args || !*args)
		return ;
	i = 0;
	while (args[i])
		i++;
	underscore = pro_strjoin("_=", args[i - 1]);
	mini_export(minishell, underscore);
	return ;
}

int	call_cmd(t_minishell *minishell, char **args)
{
	int		*status;
	char	*cmd;

	if (!args || !*args)
		return (0);
	update_underscore(minishell, args);
	cmd = *args;
	status = minishell->stat;
	if (match_str(cmd, CD))
		*status = change_dir(minishell, args);
	else if (match_str(cmd, PWD))
		*status = get_dir(minishell);
	else if (match_str(cmd, ENV))
		*status = env(minishell);
	else if (match_str(cmd, MINI_ECHO))
		*status = echo(args);
	else if (match_str(cmd, EXPORT))
		*status = export(minishell, args);
	else if (match_str(cmd, UNSET))
		*status = unset(minishell, args);
	else if (match_str(cmd, BASH_EXIT))
		*status = exit_shell(args);
	else
		lunch_bin_wraper(args, minishell, status);
	return (*status);
}

void	exec_cmd(t_exec_tree *tree, t_minishell *minishell)
{
	char		**args;

	if (!tree->argv || !tree->argv[0])
		return ;
	args = replace_args(tree->argv, minishell);
	if ((*args) && !(**args))
	{
			*(minishell->stat) = return_msg(127, "#$: command not found",
				*args);
		return ;
	}
	*(minishell->stat) = call_cmd(minishell, args);
}
