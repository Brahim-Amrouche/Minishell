/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:57:04 by maboulkh          #+#    #+#             */
/*   Updated: 2023/05/17 20:35:07 by maboulkh         ###   ########.fr       */
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

// int calc_tree_size(char **tree)
// {
// 	int	size;

// 	if (!tree)
// 		return (0);
// 	size = 0;
// 	while (tree[size])
// 		size++;
// 	return (size);
// }

// char **tree_d add_element_to_tree(char **tree, char *elem)
// {
// 	char	**res;
// 	int		size;
// 	int		i;

// 	size = calc_tree_size(tree);
// 	res = malloc((size + 1) * sizeof(char *));
// 	i = 0;
// 	while (i < size)
// 	{
// 		res[i] = tree[i];
// 		++i;
// 	}
// 	res[i] = elem;
// 	free(tree);
// 	return (res);
// }

int pow2(int n)
{
	int res;

	res = 1;
	while (n--)
		res *= 2;
	return (res);
}

void traverse_tree(t_exec_tree *tree, int depth, int offset, char **tree_d, int *tree_size, char **types)
{
	t_exec_node *node;

	if (tree->type == LOGICAL_EXEC)
	{
		node = tree->info.exec_node;
		if (depth > *tree_size)
			*tree_size = depth;
		tree_d[pow2(depth) + offset] = node->cmd[0];
		// printf("\t|%s|%d\t", node->cmd[0], depth);
		if (!ft_strncmp("exit", node->cmd[0], -1))
			exit(0);
	}
	else
	{
		tree_d[pow2(depth) + offset] = types[tree->type];
	}
	if (tree->left)
	{
		// printf("left\t");
		traverse_tree(tree->left, depth + 1, offset * 2, tree_d, tree_size, types);
	}
	if (tree->right)
	{
		// printf("right\t");
		traverse_tree(tree->right, depth + 1, offset + 1 + (2 * offset), tree_d, tree_size, types);

		printf("\n");
	}
}

 void print_tree_n(char ** tree, int depth)
 {
	int i;
	int size;
	int d;

	d = 0;
	while (d < depth + 1)
	{
		size = pow2(d + 1) - 1;
		i = 0;
		while (i < (pow2(depth - d) / 2))
		{
			printf("%5s\t", ".");
			i++;
		}
		i = pow2(d) - 1;
		while (i < size)
		{
			printf("%5s\t", tree[i]);
			i++;
		}
		printf("\n");
		d++;
	}
 }

int	exec_cmd(t_minishell *minishell)
{
	t_exec_tree *tree;
	char		**tree_d;
	char		*draw[200];
	int			tree_size;
	char		**types;
	// t_exec_node *node;
	// int depth;
	// int route;
	// t_list *token;
	// // t_list *temp;

	// token = minishell->tokens;
	// depth = 0;
	tree = minishell->exec_root;
	tree_d = NULL;
	tree_size = 0;
	ft_memset(&draw, 0, sizeof(draw));
	types = ft_split("O | && || x", ' ');
	traverse_tree(tree, 0, 0, draw, &tree_size, types);
	print_tree_n(draw, tree_size);
	// print_tree(tree);

	// if (tree->type == LOGICAL_EXEC)
	// {
	// 	node = tree->info.exec_node;
	// 	printf("cmd -> |%s|\n", node->cmd[0]);
	// 	if (!ft_strncmp("exit", node->cmd[0], -1))
	// 		exit(0);
	// }
	// else
		// printf("none\n");
	return (0);

	// t_exec_node **oper_cmds = minishell->parsed_cmds.op_cmds;
	// t_exec_node *pipe_list;
	// char		**cmd1;
	// while(*oper_cmds)
	// {
	// 	printf("new operation \n");
	// 	pipe_list = *oper_cmds;
	// 	while(pipe_list->cmd) // this is a technical issue for now and to disscuss
	// 	{
	// 		printf("new pipe args\n");
	// 		cmd1 = pipe_list->cmd;
	// 		while (*cmd1)
	// 		{
	// 			printf("args |%s|\n", *cmd1);
	// 			cmd1++;
	// 		}
	// 		pipe_list++;
	// 	}
	// 	oper_cmds++;
	// }
	// // temp = token;
	// // while (temp)
	// // {
    // //     printf("tokens ==> |%s|\n", (char *)temp->content);
    // //     temp = temp->next;
    // // }
	// if (!cmd)
	// 	exit_minishell(-1, NULL, TRUE);
	// else if (!cmd)
	// 	minishell->cmd_status = 0;
	// else if (!ft_strncmp(CD, cmd, ft_strlen(CD)))
	// 	minishell->cmd_status = change_dir(minishell, token);
	// else if (!ft_strncmp(PWD, cmd, ft_strlen(PWD)))
	// 	minishell->cmd_status = get_dir();
	// else if (!ft_strncmp(ENV, cmd, ft_strlen(ENV)))
	// 	minishell->cmd_status = env(minishell);
	// else if (!ft_strncmp(ECHO, cmd, ft_strlen(ECHO)))
	// 	minishell->cmd_status = echo(minishell, token);
	// else if (!ft_strncmp(EXPORT, cmd, ft_strlen(EXPORT)))
	// 	minishell->cmd_status = export(minishell, token->next);
	// else if (!ft_strncmp(UNSET, cmd, ft_strlen(UNSET)))
	// 	minishell->cmd_status = unset(minishell, token->next);
	// else if (!ft_strncmp(BASH_EXIT, cmd, ft_strlen(BASH_EXIT)))
	// 	exit_minishell(-1, NULL, TRUE);
	// return (minishell->cmd_status);
}
