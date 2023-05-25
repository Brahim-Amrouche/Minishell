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
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
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

int call_cmd(t_minishell *minishell, t_exec_node *node)
{
	int		*status;
	char	*cmd;
	int		id;

	if (!node || !node->cmd || !*node->cmd)
		return (0); // check later
	id = 0;
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
		*status = exit_shell(node);
	else
		id = lunch_bin(node, minishell);
	id_fetcher()[0] = id;
	wait_all(id, status);
	return (*status);
}

int traverse_tree(t_exec_tree *tree, t_minishell *minishell);

void exec_cmd(t_exec_tree *tree, t_minishell *minishell)
{
	t_exec_node *node;

	node = tree->info.exec_node;
	// if (!ft_strncmp("exit", node->cmd[0], -1))
	// 	exit(0);
	*(minishell->stat) = call_cmd(minishell, node);
}

void exec_and_or(t_exec_tree *tree, t_minishell *minishell)
{
	int	*stat;

	stat = (minishell->stat);
	if (tree->left)
		*stat = traverse_tree(tree->left, minishell);
	while (wait(NULL) != -1)
		;
	if (*stat && tree->type == LOGICAL_AND)
		return;
	else if (!*stat && tree->type == LOGICAL_OR)
		return;
	if (tree->right)
		*stat = traverse_tree(tree->right, minishell);
}

void exec_pipe(t_exec_tree *tree, t_minishell *minishell)
{
	int	f1;
	int	f2;
	int	p[2];
	int	status;
	
	pipe(p);
	f1 = fork();
	if (f1 == -1)
		exit_minishell(1, "couldn't fork", TRUE);
	id_fetcher()[0] = f1;
	if (!f1)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(p[0]);
		dup2(p[1], 1);
		close(p[1]);
		status = traverse_tree(tree->left,minishell);
		exit(status);
	}
	close(p[1]);
	f2 = fork();
	if (f2 == -1)
		exit_minishell(1, "couldn't fork", TRUE);
	id_fetcher()[1] = f2;
	if(!f2)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		dup2(p[0], 0);
		close(p[0]);
		status = traverse_tree(tree->right, minishell);
		exit(status);
	}
	close(p[0]);
	wait_all(f2, minishell->stat);
}

int traverse_tree(t_exec_tree *tree, t_minishell *minishell)
{
	int	status;

	if (!tree)
		return (0);
	status = 0;
	minishell->stat = &status;
	if (state.exec_stop)
		return (130);
	if (tree->type == LOGICAL_PIPE)
		exec_pipe(tree, minishell);
	else if (tree->type == LOGICAL_AND || tree->type == LOGICAL_OR)
		exec_and_or(tree, minishell);
	else if (tree->type == LOGICAL_EXEC)
		exec_cmd(tree, minishell);
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

int main_execution(t_minishell *minishell)
{
	t_exec_tree *tree;

	tree = minishell->exec_root;
	// traverse_and_print_tree(tree);
	minishell->cmd_status = traverse_tree(tree, minishell);
	// printf("minishell status is %d\n", minishell->cmd_status);
	return (0);
}
