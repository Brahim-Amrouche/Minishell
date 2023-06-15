/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 15:32:24 by maboulkh          #+#    #+#             */
/*   Updated: 2023/06/15 16:33:09 by maboulkh         ###   ########.fr       */
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
		while (match_str(args[0], ""))
			args++;
		if (!*args)
			exit(0);
		binary_parser(args, mini);
		if (access(args[0], F_OK) && (args[0] || args))
			exit(return_msg(127, "minishell: $: command not found", args[0]));
		else if (access(args[0], X_OK))
			exit(return_msg(127, "minishell: $: permission denied", args[0]));
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


// char	**create_args_with_wildcard(char **args)
// {
// 	char		**new_args;
// 	char		**wildcard_arr;
// 	char		*new_elem;
// 	int			i;

// 	new_args = NULL;
// 	i = 0;
// 	while (args[i])
// 	{
// 		if (*(args[i]) == '\"' || *(args[i]) == '\'' //is this condition safe // echo ""*
// 			|| !ft_strchr(args[i], '*'))
// 		{
// 			new_elem = pro_str_dup(args[i]);
// 			new_args = add_element_to_array(new_args, &new_elem, sizeof(char *));
// 		}
// 		else
// 		{
// 			wildcard_arr = create_wildcard_arr(args[i]);
// 			new_args = add_arr_to_array(new_args, wildcard_arr, sizeof(char *));
// 		}
// 		i++;
// 	}
// 	return (new_args);
// }

// t_boolean has_wildcard(char *arg)
// {
// 	size_t	i;
// 	int quote_type;

// 	i = 0;
// 	while (arg[i])
// 	{
// 		if (arg[i] == '\'')
// 			while (arg[i] && arg[i] != '\'')
// 		i++;
// 	}
	
// }

// char	**expend_args(char **args, t_minishell *minishell)
// {
// 	char		**expended;
// 	char		**wildcard;
// 	int			i;

// 	expended = NULL;
// 	i = -1;
// 	while (args[++i])
// 	{
// 		args[i] = get_var(args[i], minishell);
// 		wildcard = create_wildcard_arr(args[i], minishell);
// 		// printf("wildcard = %s\n", wildcard[1]);
// 		// if (wildcard)
// 			expended = add_arr_to_array(expended, wildcard, sizeof(char *));
// 		// else
// 		// 	add_element_to_array(expended, args + i, sizeof(char *));
// 	}
// 	i = -1;
// 	while (expended[++i])
// 	{
// 		expended[i] = unwrap_quotes(expended[i], minishell);
// 		// printf("expended = %s\n", expended[i]);
// 	}
// 	return (expended);
// }

void exec_cmd(t_exec_tree *tree, t_minishell *minishell)
{
	char		**args;
	// t_boolean	*has_wildcard;
	// int			i;

	if (!tree->argv)
		return;
	// args = tree->argv;
	// i = -1;
	// while (args[++i])
	// 	args[i] = replace_args(args[i], minishell);
	// args = create_args_with_wildcard(args);
	// tree->argv = args;
	// args = expend_args(tree->argv, minishell);
	// dprintf(2, "args = \n");
	args = replace_args(tree->argv, minishell);
	// dprintf(2, "args = %s\n", args[0]);
	*(minishell->stat) = call_cmd(minishell, args);
}