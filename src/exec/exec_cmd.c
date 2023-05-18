/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:57:04 by maboulkh          #+#    #+#             */
/*   Updated: 2023/05/18 20:31:12 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **get_env_var(char *name, char **env)
{
	int	len;

	if (!name || !env || !*env)
		return (NULL);
	len = ft_strlen(name);
	while (*env)
	{
		// if (!ft_strncmp(*env, name, len) && (*env)[len] == '=')
		if (!ft_strncmp(*env, name, len) && ((*env)[len] == '=' || !(*env)[len]))
			return (env);
		env++;
	}
	return (NULL);
}

int pow2(int n)
{
	int res;

	res = 1;
	while (n--)
		res *= 2;
	return (res);
}

static t_boolean match_cmd(char *cmd, char *match)
{
	return (!ft_strncmp(cmd, match, -1));
}

int call_cmd(t_minishell *minishell, t_exec_node *node)
{
	int		*status;
	char	*cmd;

	if (!node || !node->cmd || !*node->cmd)
		return (0); // check later
	cmd = *node->cmd;
	status = &minishell->cmd_status;
	if (match_cmd(cmd, CD))
		*status = change_dir(minishell, node);
	else if (match_cmd(cmd, PWD))
		*status = get_dir();
	else if (match_cmd(cmd, ENV))
		*status = env(minishell);
	else if (match_cmd(cmd, ECHO))
		*status = echo(minishell, node);
	else if (match_cmd(cmd, EXPORT))
		*status = export(minishell, node, 0);
	else if (match_cmd(cmd, UNSET))
		*status = unset(minishell, node, 0);
	else if (match_cmd(cmd, BASH_EXIT))
		exit_minishell(-1, NULL, TRUE);
	return (*status);
}

void traverse_tree(t_exec_tree *tree, t_minishell *minishell)
{
	t_exec_node *node;

	if (tree->type == LOGICAL_EXEC)
	{
		node = tree->info.exec_node;
		call_cmd(minishell, node);
		// printf("\t|%s|\t", node->cmd[0]);
		if (!ft_strncmp("exit", node->cmd[0], -1))
			exit(0);
	}
	if (tree->left)
		traverse_tree(tree->left, minishell);
	if (tree->right)
		traverse_tree(tree->right, minishell);
}

void make_tree(t_exec_tree *tree, int depth, int offset, char **tree_d, int *tree_size, char **types)
{
	t_exec_node *node;

	if (tree->type == LOGICAL_EXEC)
	{
		node = tree->info.exec_node;
		if (depth > *tree_size)
			*tree_size = depth;
		tree_d[pow2(depth) + offset - 1] = node->cmd[0];
		if (!ft_strncmp("exit", node->cmd[0], -1))
			exit(0);
	}
	else
	{
		tree_d[pow2(depth) + offset - 1] = types[tree->type];
	}
	if (tree->left)
	{
		make_tree(tree->left, depth + 1, 2 * offset, tree_d, tree_size, types);
	}
	if (tree->right)
	{
		make_tree(tree->right, depth + 1, 2 * offset + 1, tree_d, tree_size, types);
	}
}

void print_tree_n(char ** tree, int depth)
{
	int i;
	int j;
	int size;
	int d;

	d = 0;
	size = 1;
	while (d < depth + 1)
	{
		if (d % 2)
			printf("    ");
		i = pow2(d) - 1;
		while (i < size)
		{
			j = 0;
			while (j < (pow2(depth) / pow2(d + 1)))
			{
				printf("        ");
				j++;
			}
			if (tree[i])
				printf("%8s,%d", tree[i], i);
			else
				printf("        ");
			i++;
		}
		printf("\n");
		d++;
		size = pow2(d + 1) - 1;
	}
}

int traverse_and_print_tree(t_exec_tree *tree)
{
	char		**tree_d;
	char		*draw[200];
	int			tree_size;
	char		**types;

	tree_d = NULL;
	tree_size = 0;
	ft_memset(&draw, 0, sizeof(draw));
	types = ft_split("O | && || x", ' ');
	make_tree(tree, 0, 0, draw, &tree_size, types);
	print_tree_n(draw, tree_size);
	return (0);
}

int exec_cmd(t_minishell *minishell)
{
	t_exec_tree *tree;
	// t_exec_node *node;
	// int depth;
	// int route;
	// t_list *token;
	// // t_list *temp;

	// token = minishell->tokens;
	tree = minishell->exec_root;
	// traverse_and_print_tree(tree);
	traverse_tree(tree, minishell);
	return (0);
}
