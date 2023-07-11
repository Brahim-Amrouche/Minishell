/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_logic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 15:43:44 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/11 13:34:51 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_and_or(t_exec_tree *tree, t_minishell *minishell)
{
	int	*stat;

	stat = (minishell->stat);
	if (tree->left)
		*stat = traverse_tree(tree->left, minishell);
	while (wait(NULL) != -1)
		;
	if (*stat && tree->type == LOGICAL_AND)
		return ;
	else if (!*stat && tree->type == LOGICAL_OR)
		return ;
	if (tree->right)
		*stat = traverse_tree(tree->right, minishell);
}

void	exec_parentheses(t_exec_tree *tree, t_minishell *minishell)
{
	int	*stat;
	int	id;

	g_sigvar.in_child = TRUE;
	stat = (minishell->stat);
	if (tree->left)
	{
		id = fork();
		if (id == 0)
		{
			if (tree->left->type == LOGICAL_PARENTHESE && !tree->redir
				&& tree->left->left)
				check_parentises_syntax(tree->left->left);
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			*stat = traverse_tree(tree->left, minishell);
			exit(*stat);
		}
		wait_all(id, stat);
	}
	g_sigvar.in_child = FALSE;
}

static void	first_pipe(t_minishell *minishell, t_exec_tree *tree, int *p)
{
	int	status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(p[0]);
	dup2(p[1], 1);
	close(p[1]);
	status = traverse_tree(tree->left, minishell);
	exit(status);
}

static void	second_pipe(t_minishell *minishell, t_exec_tree *tree, int *p)
{
	int	status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	dup2(p[0], 0);
	close(p[0]);
	status = traverse_tree(tree->right, minishell);
	exit(status);
}

void	exec_pipe(t_exec_tree *tree, t_minishell *minishell)
{
	int	f1;
	int	f2;
	int	p[2];

	g_sigvar.in_child = TRUE;
	pipe(p);
	f1 = fork();
	if (f1 == -1)
		exit_minishell(1, "couldn't fork", TRUE);
	if (!f1)
		first_pipe(minishell, tree, p);
	close(p[1]);
	f2 = fork();
	if (f2 == -1)
		exit_minishell(1, "couldn't fork", TRUE);
	if (!f2)
		second_pipe(minishell, tree, p);
	close(p[0]);
	wait_all(f2, minishell->stat);
	g_sigvar.in_child = FALSE;
}
