/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:57:04 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/11 13:34:51 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	reset_std(int *std)
{
	if (std[STDIN_FILENO] >= 0)
	{
		dup2(std[STDIN_FILENO], STDIN_FILENO);
		close(std[STDIN_FILENO]);
	}
	if (std[STDOUT_FILENO] >= 0)
	{
		dup2(std[STDOUT_FILENO], STDOUT_FILENO);
		close(std[STDOUT_FILENO]);
	}
}

static t_stat	open_all_redir(t_redir_info **tree_redir,
		t_minishell *minishell, int *tree_std)
{
	while (tree_redir && *tree_redir)
	{
		if (handle_redirection(*tree_redir, minishell, tree_std))
		{
			reset_std(tree_std);
			return (FAIL);
		}
		tree_redir++;
	}
	return (SUCCESS);
}

int	traverse_tree(t_exec_tree *tree, t_minishell *minishell)
{
	int	status;
	int	tree_std[2];

	if (!tree)
		return (0);
	ft_memset(tree_std, 255, sizeof(tree_std));
	status = 0;
	minishell->stat = &status;
	if (g_sigvar.exec_stop)
		return (STOP_WITH_SIGINT);
	if (open_all_redir(tree->redir, minishell, tree_std))
		return (status);
	if (tree->type == LOGICAL_PIPE)
		exec_pipe(tree, minishell);
	else if (tree->type == LOGICAL_AND || tree->type == LOGICAL_OR)
		exec_and_or(tree, minishell);
	else if (tree->type == LOGICAL_PARENTHESE)
		exec_parentheses(tree, minishell);
	else if (tree->type == LOGICAL_EXEC)
		exec_cmd(tree, minishell);
	reset_std(tree_std);
	return (status);
}

int	main_execution(t_minishell *minishell)
{
	t_exec_tree	*tree;

	tree = minishell->exec_root;
	if (read_here_docs(tree, minishell))
	{
		minishell->cmd_status = 1;
		g_sigvar.exec_stop = FALSE;
		return (0);
	}
	minishell->cmd_status = traverse_tree(tree, minishell);
	return (0);
}
