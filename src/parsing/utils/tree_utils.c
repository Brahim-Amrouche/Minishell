/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 14:03:14 by bamrouch          #+#    #+#             */
/*   Updated: 2023/06/01 16:32:51 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	loop_exec_tree(t_exec_tree *root, t_minishell *mini,
		void (*exec)(t_exec_tree *, t_minishell *))
{
	if (!root)
		return ;
	loop_exec_tree(root->left, mini, exec);
	loop_exec_tree(root->right, mini, exec);
	exec(root, mini);
}

t_exec_tree	*exec_tree_node(int params_num, ...)
{
	va_list		args;
	t_exec_tree	*tree_node;

	tree_node = ft_malloc(sizeof(t_exec_tree), m_info(NULL, 1, NULL, 0));
	if (!tree_node)
		exit_minishell(ENOMEM, "couldn't malloc a tree_node", TRUE);
	ft_bzero(tree_node, sizeof(t_exec_tree));
	va_start(args, params_num);
	if (params_num >= 1)
		tree_node->type = va_arg(args, t_logical_operators);
	if (params_num >= 2)
		tree_node->parent = va_arg(args, t_exec_tree *);
	if (params_num >= 3)
		tree_node->left = va_arg(args, t_exec_tree *);
	if (params_num >= 4)
		tree_node->right = va_arg(args, t_exec_tree *);
	// if (params_num >= 5)
	// 	tree_node->info = va_arg(args, t_exec_info);
	va_end(args);
	return (tree_node);
}
