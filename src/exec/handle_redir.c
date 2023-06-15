/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 15:14:24 by maboulkh          #+#    #+#             */
/*   Updated: 2023/06/15 16:44:45 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_stat handle_redir_fd(int fd, t_redir_info *redir, int *std)
{
	int	std_fileno;

	// if ((type == INPUT_REDI && std[0] >= 0)
	// 	|| (type == OUTPUT_REDI && std[1] >= 0))
	// {
	// 	close(fd);
	// 	return (SUCCESS);
	// }
	if (redir->redir_type == INPUT_REDI || redir->redir_type == HERE_DOC_REDI)
		std_fileno = STDIN_FILENO;
	else
		std_fileno = STDOUT_FILENO;
	if (std[std_fileno] < 0)
		std[std_fileno] = dup(std_fileno);
	// if (minishell->std[std_fileno] < 0)
	// 	minishell->std[std_fileno] = std[std_fileno];
    dup2(fd, std_fileno);
    close(fd);
	return (SUCCESS);
}

static char	*expend_heredoc(char *arg, t_minishell *mini)
{
	arg = get_var(arg, mini);
	arg = unwrap_quotes(arg, mini);
	return (arg);
}

static t_stat handle_heredoc(t_redir_info *redir, t_minishell *minishell,
					int *tree_std)
{
	char	*limiter;
	char	**heredoc_content;
	int		p[2];

	limiter = redir->content;
	if (pipe(p) == -1)
		exit_minishell(1, "couldnt open pipe", TRUE);
	heredoc_content = redir->heredoc_content;
	while (heredoc_content && *heredoc_content)
	{
		if (redir->has_quotes == FALSE)
			*heredoc_content = expend_heredoc(*heredoc_content, minishell);
		write(p[1], *heredoc_content, ft_strlen(*heredoc_content));
		heredoc_content++;
	}
	if (close(p[1]))
		print_msg(2, "minishell: $ (heredoc): can't be closed", limiter);
	if (redir->redir_type == HERE_DOC_REDI)
		redir->redir_type = INPUT_REDI;
	handle_redir_fd(p[0], redir, tree_std);
	return (SUCCESS);
}

static t_stat check_redir_access(t_redirection_types redir_type,
					char *path, int *stat)
{
	if (access(path, F_OK) == 0)
	{
		if ((redir_type == INPUT_REDI && access(path, R_OK))
			|| (redir_type == OUTPUT_REDI && access(path, W_OK)))
		{
			*stat = ERR_NO_RED_P;
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

static int get_redir_flag(t_redirection_types redir_type)
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

t_stat	replace_redir_args(t_redir_info *redir, t_minishell *minishell)
{
	char **redir_arr;
	int i;

	redir_arr = NULL;
	redir_arr = add_element_to_array(redir_arr,
					&(redir->content), sizeof(char *));
	redir_arr = replace_args(redir_arr, minishell);
	i = 0;
	while (redir_arr && redir_arr[i])
		i++;
	if (i > 1)
	{
		*(minishell->stat) = return_msg(1,
			"minishell: $: ambiguous redirect", redir->content);
		return (FAIL);
	}
	redir->content = redir_arr[0];
	return (SUCCESS);
}

t_stat	handle_redirection(t_redir_info *redir, t_minishell *minishell,
			int *tree_std)
{
	int flag;
	int fd;
	int *stat;
	// char **wildcard_arr;
	// int i;

	// wildcard_arr = NULL;
	stat = minishell->stat;
	if (redir->redir_type == HERE_DOC_REDI)
		return (handle_heredoc(redir, minishell, tree_std));
	if (replace_redir_args(redir, minishell) == FAIL)
		return (FAIL);
	flag = get_redir_flag(redir->redir_type);
	if (redir->redir_type == APPEND_REDI)
		redir->redir_type = OUTPUT_REDI;// does it matter if i cahnge this here
	if (check_redir_access(redir->redir_type, redir->content, stat))
		return (FAIL);
	fd = open(redir->content, flag, 0644);
	if (fd < 0)
	{
		*(minishell->stat) = 1;
		print_msg(2, "minishell: $: can't be open", redir->content);
		return (FAIL);
	}
	handle_redir_fd(fd, redir, tree_std);
	return (SUCCESS);

	// if (*(redir->content) != '\"' && *(redir->content) != '\''
	// 		&& ft_strchr(redir->content, '*'))
	// 	wildcard_arr = create_wildcard_arr(redir->content, minishell);
	// i = 0;
	// while (wildcard_arr && wildcard_arr[i])
	// 	i++;
	// if (i > 1)
	// {
	// 	*(minishell->stat) = return_msg(1, "minishell: $: ambiguous redirect", redir->content);
	// 	return (FAIL);
	// }
	// redir->content = wildcard_arr[0];

	// redir->content = replace_args(redir->content, minishell);
}
