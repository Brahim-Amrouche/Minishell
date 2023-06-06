/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 15:32:24 by maboulkh          #+#    #+#             */
/*   Updated: 2023/06/06 15:38:41 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int lunch_bin(char **args, t_minishell *mini)
{
	int		id;

	id = fork();
	if (id == -1)
		exit_minishell(1, "could't fork", TRUE);
	if (id == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		binary_parser(args, mini);
		if (access(args[0], F_OK) && (args[0] || args))
			exit_minishell(127, "command not found", FALSE);
		else if (access(args[0], X_OK))
			exit_minishell(126, "permission denied", FALSE);
		else
			execve(args[0], args, mini->envp);
		exit(126);
	}
	return (id);
}

int call_cmd(t_minishell *minishell, char **args)
{
	int		*status;
	char	*cmd;
	int		id;

	if (!args || !*args)
		return (0); // check later
	id = 0;
	cmd = *args;
	status = minishell->stat;
	if (match_str(cmd, CD))
		*status = change_dir(minishell, args);
	else if (match_str(cmd, PWD))
		*status = get_dir();
	else if (match_str(cmd, ENV))
		*status = env(minishell);
	else if (match_str(cmd, MINI_ECHO))
		*status = echo(args);
	else if (match_str(cmd, EXPORT))
		*status = export(minishell, args, 0);
	else if (match_str(cmd, UNSET))
		*status = unset(minishell, args, 0);
	else if (match_str(cmd, BASH_EXIT))
		*status = exit_shell(args);
	else
	{
		id = lunch_bin(args, minishell);
		(*get_sigvar()).in_child = TRUE;
	}
	wait_all(id, status);
	(*get_sigvar()).in_child = FALSE;
	return (*status);
}


char	**create_args_with_wildcard(char **args)
{
	char		**new_args;
	char		**wildcard_arr;
	char		*new_elem;
	int			i;

	new_args = NULL;
	i = 0;
	while (args[i])
	{
		if (*(args[i]) == '\"' || *(args[i]) == '\''
			|| !ft_strchr(args[i], '*'))
		{
			new_elem = pro_str_dup(args[i]);
			new_args = add_element_to_array(new_args, &new_elem, sizeof(char *));
		}
		else
		{
			wildcard_arr = create_wildcard_arr(args[i]);
			new_args = add_arr_to_array(new_args, wildcard_arr, sizeof(char *));
		}
		i++;
	}
	return (new_args);
}

void exec_cmd(t_exec_tree *tree, t_minishell *minishell)
{
	char		**args;
	int			i;

	if (!tree->argv)
		return;
	args = create_args_with_wildcard(tree->argv);
	i = -1;
	while (args[++i])
		args[i] = replace_args(args[i], minishell);
	tree->argv = args;
	*(minishell->stat) = call_cmd(minishell, args);
}