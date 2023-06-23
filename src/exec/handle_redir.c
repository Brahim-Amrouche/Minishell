/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 15:14:24 by maboulkh          #+#    #+#             */
/*   Updated: 2023/06/23 14:03:24 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_stat	handle_redir_fd(int fd, t_redir_info *redir, int *std)
{
	int	std_fileno;

	if (redir->redir_type == INPUT_REDI || redir->redir_type == HERE_DOC_REDI)
		std_fileno = STDIN_FILENO;
	else
		std_fileno = STDOUT_FILENO;
	if (std[std_fileno] < 0)
		std[std_fileno] = dup(std_fileno);
	dup2(fd, std_fileno);
	close(fd);
	return (SUCCESS);
}

static t_stat	handle_heredoc(t_redir_info *redir, t_minishell *minishell,
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
			*heredoc_content = get_var(*heredoc_content, minishell, FALSE);
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

static t_stat	check_redir_access(t_redirection_types redir_type,
									char *path,
									int *stat)
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

static int	get_redir_flag(t_redirection_types redir_type)
{
	int	flag;

	flag = 0;
	if (redir_type == INPUT_REDI)
		flag = O_RDONLY | O_SYMLINK;
	else if (redir_type == OUTPUT_REDI)
		flag = O_WRONLY | O_CREAT | O_TRUNC | O_SYMLINK;
	else if (redir_type == APPEND_REDI)
		flag = O_WRONLY | O_CREAT | O_APPEND | O_SYMLINK;
	return (flag);
}

t_stat	replace_redir_args(t_redir_info *redir, t_minishell *minishell)
{
	char	**redir_arr;
	char	*redir_name;
	int		i;

	redir_name = pro_str_dup(redir->content);
	redir_arr = NULL;
	redir_arr = add_element_to_array(redir_arr, &(redir->content),
										sizeof(char *));
	redir_arr = replace_args(redir_arr, minishell);
	i = 0;
	while (redir_arr && redir_arr[i])
		i++;
	if (i > 1)
	{
		*(minishell->stat) = return_msg(1,
										"minishell: $: ambiguous redirect",
										redir_name);
		return (FAIL);
	}
	ft_free_node(1, redir_name);
	redir->content = redir_arr[0];
	return (SUCCESS);
}

t_stat	handle_redirection(t_redir_info *redir, t_minishell *minishell,
		int *tree_std)
{
	int	flag;
	int	fd;
	int	*stat;

	stat = minishell->stat;
	if (redir->redir_type == HERE_DOC_REDI)
		return (handle_heredoc(redir, minishell, tree_std));
	if (replace_redir_args(redir, minishell) == FAIL)
		return (FAIL);
	flag = get_redir_flag(redir->redir_type);
	if (redir->redir_type == APPEND_REDI)
		redir->redir_type = OUTPUT_REDI; // does it matter if i cahnge this here
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
}
