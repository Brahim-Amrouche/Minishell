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

int lunch_bin(t_exec_node *node, t_minishell *mini)
{
	char	**cmd;
	int		id;

	id = fork();
	if (id == -1)
		exit_minishell(1, "could't fork", TRUE);
	if (id == 0)
	{
		cmd = node->cmd;
		binary_parser(cmd, mini);
		if (access(cmd[0], F_OK) && (cmd[0] || cmd))
			exit_minishell(127, "command not found", FALSE);
		else if (access(cmd[0], X_OK))
			exit_minishell(126, "permission denied", FALSE);
		else
			execve(cmd[0], cmd, mini->envp);
		exit(126);
	}
	return (id);
}

static t_boolean match_cmd(char *cmd, char *match)
{
	return (!ft_strncmp(cmd, match, -1));
}

typedef enum e_tree_side
{
	RIGHT = 0,
	LEFT = 1,
}	t_tree_side;

typedef struct s_pipes
{
	int	p1[2];
	int	p2[2];
}	t_pipes;

typedef struct s_exec_pipe
{
	t_tree_side	side;
	t_pipes		*pipe;
	int			*id;
	int			pipe_lvl;
}	t_exec_pipe;

static void	wait_all(pid_t last_proc, int *status)
{
	pid_t	id;
	int		stat;

	id = wait(&stat);
	if (id == last_proc)
		*status = stat / 256;
	if (id != -1)
		wait_all(last_proc, status);
}

int call_cmd(t_minishell *minishell, t_exec_node *node, t_exec_pipe info)
{
	int		*status;
	char	*cmd;
	t_boolean is_bin;

	if (!node || !node->cmd || !*node->cmd)
		return (0); // check later
	is_bin = FALSE;
	cmd = *node->cmd;
	status = minishell->stat;
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
	else
	{
		is_bin = TRUE;
		*(info.id) = lunch_bin(node, minishell);
	}
	if ((is_bin && info.pipe_lvl == 0) || (info.pipe_lvl == 1 && info.side == RIGHT))
		wait_all(*(info.id), status);
	return (*status);
}

static void	switch_pipes(t_exec_pipe info)
{
	ft_memmove(&(info.pipe->p1), &(info.pipe->p1), sizeof(info.pipe->p1));
	return ;
}

int pipe_stdin(void)
{

}

int pipe_stdout(void)
{

}

int	piping(t_exec_pipe info)
{
	if (info.pipe_lvl == 0)
		return (0);
	else if (info.pipe_lvl == 1 && info.side == LEFT)
		pipe_stdin();
	else if (info.pipe_lvl == 1 && info.side == RIGHT)
		pipe_stdout();
	else
		switch_pipes(info);
	// if (cmd_number != pipex->cmd_nbr - 1)
	// 	if (pipe(pipex->pipes.p2) == -1)
	// 		exit_with_msg("couldnt open pipe", TRUE);
}

int traverse_tree(t_exec_tree *tree, t_minishell *minishell, t_exec_pipe info);

int	create_waiting_proc(t_exec_tree *tree, t_minishell *minishell, t_exec_pipe info)
{
	int status;
	int id;

	id = fork();
	if (id == -1)
		exit_minishell(1, "could't fork", TRUE);
	if (id == 0)
	{
		status = 0;
		*minishell->stat = status;
		traverse_tree(tree, minishell, info);
		exit(status);
	}
	return (id);
}

int traverse_tree(t_exec_tree *tree, t_minishell *minishell, t_exec_pipe info)
{
	t_exec_node *node;
	int			id;
	int			status;

	status = 0;
	minishell->stat = &status;
	if (tree->type == LOGICAL_PIPE)
		(info.pipe_lvl)++;
	if (tree->type == LOGICAL_EXEC)
	{
		info.id = &id;
		piping(info);
		node = tree->info.exec_node;
		status = call_cmd(minishell, node, info);
		// printf("status after call cmd is %d\n", status);
		if (!ft_strncmp("exit", node->cmd[0], -1))
			exit(0);
	}
	if (info.pipe_lvl && (tree->type == LOGICAL_AND || tree->type == LOGICAL_OR))
	{
		info.pipe_lvl = 0;
		info.side = RIGHT;
		create_waiting_proc(tree, minishell, info);
		return (0);
	}
	if (tree->left)
	{
		info.side = LEFT;
		status = traverse_tree(tree->left, minishell, info);
	}
	if (tree->type == LOGICAL_AND || tree->type == LOGICAL_OR)
	{
		printf("or or and\n");
		while (wait(NULL) != -1)
			;
		if (status && tree->type == LOGICAL_AND)
			return (status);
		else if (!status && tree->type == LOGICAL_OR)
			return (status);
	}
		// printf("status after left is %d\n", status);
	if (tree->right)
	{
		info.side = RIGHT;
		status = traverse_tree(tree->right, minishell, info);
	}
		// printf("status after right is %d\n", status);
	return (status);
}

// void make_tree(t_exec_tree *tree, int depth, int offset, char **tree_d, int *tree_size, char **types)
// {
// 	t_exec_node *node;

// 	if (tree->type == LOGICAL_EXEC)
// 	{
// 		node = tree->info.exec_node;
// 		if (depth > *tree_size)
// 			*tree_size = depth;
// 		tree_d[pow2(depth) + offset - 1] = node->cmd[0];
// 		if (!ft_strncmp("exit", node->cmd[0], -1))
// 			exit(0);
// 	}
// 	else
// 	{
// 		tree_d[pow2(depth) + offset - 1] = types[tree->type];
// 	}
// 	if (tree->left)
// 	{
// 		make_tree(tree->left, depth + 1, 2 * offset, tree_d, tree_size, types);
// 	}
// 	if (tree->right)
// 	{
// 		make_tree(tree->right, depth + 1, 2 * offset + 1, tree_d, tree_size, types);
// 	}
// }

// void print_tree_n(char ** tree, int depth)
// {
// 	int i;
// 	int j;
// 	int size;
// 	int d;

// 	d = 0;
// 	size = 1;
// 	while (d < depth + 1)
// 	{
// 		if (d % 2)
// 			printf("    ");
// 		i = pow2(d) - 1;
// 		while (i < size)
// 		{
// 			j = 0;
// 			while (j < (pow2(depth) / pow2(d + 1)))
// 			{
// 				printf("        ");
// 				j++;
// 			}
// 			if (tree[i])
// 				printf("%8s,%d", tree[i], i);
// 			else
// 				printf("        ");
// 			i++;
// 		}
// 		printf("\n");
// 		d++;
// 		size = pow2(d + 1) - 1;
// 	}
// }

// int traverse_and_print_tree(t_exec_tree *tree)
// {
// 	char		**tree_d;
// 	char		*draw[200];
// 	int			tree_size;
// 	char		**types;

// 	tree_d = NULL;
// 	tree_size = 0;
// 	ft_memset(&draw, 0, sizeof(draw));
// 	types = ft_split("O | && || x", ' ');
// 	make_tree(tree, 0, 0, draw, &tree_size, types);
// 	print_tree_n(draw, tree_size);
// 	return (0);
// }

int exec_cmd(t_minishell *minishell)
{
	t_exec_tree *tree;
	t_exec_pipe	pipe_info;
	t_pipes	pipes;
	// t_exec_node *node;
	// int depth;
	// int route;
	// t_list *token;
	// // t_list *temp;

	// token = minishell->tokens;
	ft_memset(&pipe_info, 0, sizeof(pipe_info));
	pipe_info.pipe = &pipes;
	tree = minishell->exec_root;
	// traverse_and_print_tree(tree);
	minishell->cmd_status = traverse_tree(tree, minishell, pipe_info);
	// printf("minishell status is %d\n", minishell->cmd_status);
	return (0);
}
