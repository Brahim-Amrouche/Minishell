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
	wait_all(id, status);
	return (*status);
}

int open_heredoc(char *limiter, int *std, int *stat)
{
	char	*line;
	int		p[2];
	int		id;

	if (pipe(p) == -1)
		exit_minishell(1, "couldnt open pipe", TRUE);
	id = fork();
	if (!id)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (close(p[0]))
			print_msg(2, "minishell: $ (heredoc): can't be closed", limiter);
		while (TRUE)
		{
			line = readline("> ");
			ft_malloc(1, m_info(line, 1, NULL, 0));
			if (line && !ft_strncmp(line, limiter, ft_strlen(limiter)))
				break ;
			write(p[1], line, ft_strlen(line));
			write(p[1], "\n", 1);
		}
		if (close(p[1]))
			print_msg(2, "minishell: $ (heredoc): can't be closed", limiter);
		exit(0);
	}
	waitpid(id, stat, 0);
	if (close(p[1]))
		print_msg(2, "minishell: $ (heredoc): can't be closed", limiter);
	return (p[0]);
}

t_stat	handle_redirection(t_redirections *input, int	*std, int *stat)
{
	int flag;
	int fd;

	flag = 0;
	if (input->is_heredoc)
	{
		fd = open_heredoc(input->content, std, stat);
		if (input->continue_redirs && (input + 1)->content)
		{
			close(fd);
			return (SUCCESS);
		}
		std[0] = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
		close(fd);
		return (SUCCESS);
	}
	else if (input->is_read)
		flag = O_RDONLY;
	else if (input->is_write)
		flag = O_WRONLY | O_CREAT | O_TRUNC;
	else if (input->is_append)
	{
		flag = O_WRONLY | O_CREAT | O_APPEND;
		input->is_write = TRUE;
	}
	if (access(input->content, F_OK) == 0)
	{
		if ((input->is_read && access(input->content, R_OK))
			|| (input->is_write && access(input->content, W_OK)))
		{
			*stat = ERR_NO_P;
			print_msg(2, "minishell: $: Permission denied", input->content);
			return (FAIL);
		}
	}
	else if (!input->is_write)
	{
		*stat = ERR_NO_F;
		print_msg(2, "minishell: $: No such file or directory", input->content);
		return (FAIL);
	}
	fd = open(input->content, flag, 0644);
	if (fd < 0)
	{
		*stat = 1;
		print_msg(2, "minishell: $: can't be open", input->content);
		return (FAIL);
	}
	if (input->continue_redirs && (input + 1)->content)
	{
		close(fd);
		return (SUCCESS);
	}
	if (input->is_read)
	{
		std[0] = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (input->is_write)
	{
		std[1] = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (SUCCESS);
}

t_stat	redirect_cmd(t_redirections *input, int	*std, int *stat)
{
	if (!input || !(input->content) || *stat)
		return (SUCCESS);
	while (TRUE)
	{
		if (handle_redirection(input, std, stat))
			return (FAIL);
		if (!input->continue_redirs)
			break;
		input++;
		if (!input->content)
			break;
	}
	return (SUCCESS);
}

int traverse_tree(t_exec_tree *tree, t_minishell *minishell);

void exec_cmd(t_exec_tree *tree, t_minishell *minishell)
{
	t_exec_node *node;
	int			std[2];
	int			err;

	err = 0;
	std[0] = -1;
	std[1] = -1;
	node = tree->info.exec_node;
	err += redirect_cmd(node->input, std, minishell->stat);
	err += redirect_cmd(node->output, std, minishell->stat);
	if (!err)
		*(minishell->stat) = call_cmd(minishell, node);
	if (std[0] >= 0)
	{
		dup2(std[0], STDIN_FILENO);
		close(std[0]);
	}
	if (std[1] >= 0)
	{
		dup2(std[1], STDOUT_FILENO);
		close(std[1]);
	}
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
	if ((*get_sigvar()).exec_stop)
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
