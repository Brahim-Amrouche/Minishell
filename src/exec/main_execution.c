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
		binary_parser(args, mini);
		if (access(args[0], F_OK) && (args[0] || args))
			exit_minishell(127, "command not found", FALSE);
		else if (access(args[0], X_OK))
			exit_minishell(126, "permission denied", FALSE);
		else
			execve(args[0], args, mini->envp);
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
	if (match_cmd(cmd, CD))
		*status = change_dir(minishell, args);
	else if (match_cmd(cmd, PWD))
		*status = get_dir();
	else if (match_cmd(cmd, ENV))
		*status = env(minishell);
	else if (match_cmd(cmd, MINI_ECHO))
		*status = echo(args);
	else if (match_cmd(cmd, EXPORT))
		*status = export(minishell, args, 0);
	else if (match_cmd(cmd, UNSET))
		*status = unset(minishell, args, 0);
	else if (match_cmd(cmd, BASH_EXIT))
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


t_stat handle_redir_fd(int fd, t_redirection_types type, int *std)
{
	int	std_fileno;

	// if ((type == INPUT_REDI && std[0] >= 0)
	// 	|| (type == OUTPUT_REDI && std[1] >= 0))
	// {
	// 	close(fd);
	// 	return (SUCCESS);
	// }
	if (type == INPUT_REDI || type == HERE_DOC_REDI)
		std_fileno = STDIN_FILENO;
	else
		std_fileno = STDOUT_FILENO;
	if (std[std_fileno] < 0)
	{
		std[std_fileno] = dup(std_fileno);
	}
	dup2(fd, std_fileno);
	close(fd);
	return (SUCCESS);
}

int open_heredoc(char	*limiter, int *p, int *tree_std)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	if (tree_std[0] >= 0)
		dup2(tree_std[0], STDIN_FILENO);
	if (close(p[0]))
		print_msg(2, "minishell: $ (heredoc): can't be closed", limiter);
	while (TRUE)
	{
		line = readline("> ");
		ft_malloc(1, m_info(line, 1, NULL, 0));
		if (!line)
		{
			print_msg(2, "minishell: warning: here-document delimited by end-of-file (wanted `$')", limiter);
			break ;
		}
		if (match_cmd(line, limiter))
			break ;
		write(p[1], line, ft_strlen(line));
		write(p[1], "\n", 1);
	}
	if (close(p[1]))
		print_msg(2, "minishell: $ (heredoc): can't be closed", limiter);
	exit(0);
}

t_stat handle_heredoc(t_redir_info *redir, t_minishell *minishell, int *tree_std)
{
	char	*limiter;
	int		p[2];
	int		id;

	limiter = redir->content;
	if (pipe(p) == -1)
		exit_minishell(1, "couldnt open pipe", TRUE);
	id = fork();
	if (!id)
		open_heredoc(limiter, p, tree_std);
	waitpid(id, minishell->stat, 0);
	if (close(p[1]))
		print_msg(2, "minishell: $ (heredoc): can't be closed", limiter);
	if (redir->redir_type == HERE_DOC_REDI)
		redir->redir_type = INPUT_REDI;
	handle_redir_fd(p[0], redir->redir_type, tree_std);
	return (SUCCESS);
}

t_stat check_redir_access(t_redirection_types redir_type, char *path, int *stat)
{
	// if (redir_type == APPEND_REDI)
	// 	redir_type = OUTPUT_REDI;
	if (access(path, F_OK) == 0)
	{
		if ((redir_type == INPUT_REDI && access(path, R_OK))
			|| (redir_type == OUTPUT_REDI && access(path, W_OK)))
		{
			*stat = ERR_NO_P;
			print_msg(2, "minishell: $: Permission denied", path);
			return (FAIL);
		}
	}
	else if (redir_type != OUTPUT_REDI)
	{
		*stat = ERR_NO_F;
		print_msg(2, "minishell: $: No such file or directory", path);
		return (FAIL);
	}
	return (SUCCESS);
}

int get_redir_flag(t_redirection_types redir_type)
{
	int flag;

	flag = 0;
	if (redir_type == INPUT_REDI)
		flag = O_RDONLY;
	else if (redir_type == OUTPUT_REDI)
		flag = O_WRONLY | O_CREAT | O_TRUNC;
	else if (redir_type ==  APPEND_REDI)
		flag = O_WRONLY | O_CREAT | O_APPEND;
	return (flag);
}

t_stat	handle_redirection(t_redir_info *redir, t_minishell *minishell, int *tree_std)
{
	int flag;
	int fd;
	int *stat;

	stat = minishell->stat;
	if (redir->redir_type == HERE_DOC_REDI)
		return (handle_heredoc(redir, minishell, tree_std));
	flag = get_redir_flag(redir->redir_type);
	if (redir->redir_type == APPEND_REDI)
		redir->redir_type = OUTPUT_REDI;// does it matter if i cahnge this here
	if (check_redir_access(redir->redir_type, redir->content, stat))
		return (FAIL);
	fd = open(redir->content, flag, 0644);
	if (fd < 0)
	{
		*(minishell->stat) = 1;
		print_msg(2, "minishell: $: can't be open", *(redir->content));
		return (FAIL);
	}
	handle_redir_fd(fd, redir->redir_type, tree_std);
	return (SUCCESS);
}

int	traverse_tree(t_exec_tree *tree, t_minishell *minishell);

char	**create_args_with_wildcard(char **args)
{
	char		**new_args;
	char		**wildcard_arr;
	char		*new_elem;
	int			i;

	new_args = NULL;
	i = 0;
	while (args[i])
	{
		if (*(args[i]) == '\"' || *(args[i]) == '\''
			|| !ft_strchr(args[i], '*'))
		{
			new_elem = pro_str_dup(args[i]);
			new_args = add_element_to_array(new_args, &new_elem, sizeof(char *));
		}
		else
		{
			wildcard_arr = create_wildcard_arr(args[i]);
			new_args = add_arr_to_array(new_args, wildcard_arr, sizeof(char *));
		}
		i++;
	}
	return (new_args);
}

void exec_cmd(t_exec_tree *tree, t_minishell *minishell)
{
	char		**args;
	int			i;

	if (!tree->argv)
		return;
	args = create_args_with_wildcard(tree->argv);
	i = -1;
	while (args[++i])
		args[i] = replace_args(args[i], minishell);
	tree->argv = args;
	*(minishell->stat) = call_cmd(minishell, args);
}

// void exec_redir(t_exec_tree *tree, t_minishell *minishell)
// {
// 	int			*std;
// 	int			*stat;

// 	stat = (minishell->stat);
// 	if (!tree->argv || *stat)
// 		return ;
// 	std = minishell->redir->std;
// 	// if (handle_redirection(node, minishell) == SUCCESS)
// 		*stat = traverse_tree(tree->left, minishell);
// 	// if (std[0] >= 0)
// 	// {
// 	// 	dup2(std[0], STDIN_FILENO);
// 	// 	close(std[0]);
// 	// 	std[0] = -1;
// 	// }
// 	// if (std[1] >= 0)
// 	// {
// 	// 	dup2(std[1], STDOUT_FILENO);
// 	// 	close(std[1]);
// 	// 	std[1] = -1;
// 	// }
// }

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

void reset_std(int *std)
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

int traverse_tree(t_exec_tree *tree, t_minishell *minishell)
{
	int				status;
	t_redir_info	**tree_redir;
	int 			tree_std[2];

	if (!tree)
		return (0);
	tree_std[0] = -1;
	tree_std[1] = -1;
	status = 0;
	minishell->stat = &status;
	tree_redir = tree->redir;
	while (tree_redir && *tree_redir)
	{
		if (handle_redirection(*tree_redir, minishell, tree_std))
			return (status);
		tree_redir++;
	}
	if ((*get_sigvar()).exec_stop)
		return (130);
	if (tree->type == LOGICAL_PIPE)
		exec_pipe(tree, minishell);
	else if (tree->type == LOGICAL_AND || tree->type == LOGICAL_OR)
		exec_and_or(tree, minishell);
	else if (tree->type == LOGICAL_EXEC)
		exec_cmd(tree, minishell);
	reset_std(tree_std);
	return (status);
}


// void make_tree(t_exec_tree *tree, int depth, int offset, char **tree_d, int *tree_size, char **types)
// {
// 	t_exec_info *node;

// 	if (tree->type == LOGICAL_EXEC)
// 	{
// 		node = tree->info.exec_node;
// 		if (depth > *tree_size)
// 			*tree_size = depth;
// 		tree_d[pow2(depth) + offset - 1] = node->content[0];
// 		if (!ft_strncmp("exit", node->content[0], -1))
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
	// // traverse_and_print_tree(tree);
	minishell->cmd_status = traverse_tree(tree, minishell);
	// printf("minishell status is %d\n", minishell->cmd_status);
	return (0);
}
