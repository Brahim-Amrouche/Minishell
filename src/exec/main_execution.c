// /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:57:04 by maboulkh          #+#    #+#             */
/*   Updated: 2023/05/20 17:25:05 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void reset_std(int *std)
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

static t_stat	open_all_redir(t_redir_info **tree_redir, t_minishell *minishell, int *tree_std)
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

int traverse_tree(t_exec_tree *tree, t_minishell *minishell)
{
	int				status;
	int 			tree_std[2];
	t_boolean		origin_stdin;

	if (!tree)
		return (0);

	origin_stdin = FALSE;
	if (minishell->std < 0)
		origin_stdin = TRUE;

	tree_std[0] = -1;
	tree_std[1] = -1;
	status = 0;
	minishell->stat = &status;
	if (open_all_redir(tree->redir, minishell, tree_std))
	{
		if (origin_stdin)
		{
			minishell->std[0] = -1;
			minishell->std[1] = -1;
		}
		return (status);
	}

	if ((*get_sigvar()).exec_stop)
		return (130);
	if (tree->type == LOGICAL_PIPE)
		exec_pipe(tree, minishell);
	else if (tree->type == LOGICAL_AND || tree->type == LOGICAL_OR)
		exec_and_or(tree, minishell);
	else if (tree->type == LOGICAL_PARENTHESE)
		exec_parentheses(tree, minishell);
	else if (tree->type == LOGICAL_EXEC)
		exec_cmd(tree, minishell);
	reset_std(tree_std);
	if (origin_stdin)
	{
		minishell->std[0] = -1;
		minishell->std[1] = -1;
	}
	wait_all(0, NULL);
	return (status);
}




int main_execution(t_minishell *minishell)
{
	t_exec_tree *tree;
	// minishell->std[0] = dup(0);


	minishell->std[0] = -1;
	minishell->std[1] = -1;
	tree = minishell->exec_root;
	read_here_docs(tree, minishell);
	// // traverse_and_print_tree(tree);
	minishell->cmd_status = traverse_tree(tree, minishell);
	// printf("minishell status is %d\n", minishell->cmd_status);
	return (0);
}
