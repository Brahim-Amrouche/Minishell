/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elasce <elasce@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 14:13:35 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/10 15:01:53 by elasce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_heredoc(char *limiter, int *p)
{
	int		id;	
	char	*line;

	id = fork();
	if (id == -1)
		exit_minishell(1, "couldnt fork for heredoc", TRUE);
	if (!id)
	{
		signal(SIGINT, SIG_DFL);
		limiter = unwrap_quotes(limiter);
		close(p[0]);
		while (TRUE)
		{
			line = readline("> ");
			ft_malloc(1, m_info(line, 1, NULL, 0));
			if (!line || match_str(line, limiter))
				break ;
			write(p[1], line, ft_strlen(line));
			write(p[1], "\n", 1);
			ft_free_node(1, line);
		}
		close(p[1]);
		exit(0);
	}
	return (id);
}

static t_stat	save_heredoc_content(t_redir_info *redir,
		t_minishell *minishell)
{
	char	**temp;
	char	*line;
	int		p[2];

	redir->has_quotes = has_quotes(redir->content);
	if (pipe(p) == -1)
		exit_minishell(1, "couldnt open pipe", TRUE);
	waitpid(open_heredoc(redir->content, p), minishell->stat, 0);
	if (close(p[1]))
		print_msg(2, "minishell: $: can't be closed", redir->content);
	redir->heredoc_content = NULL;
	while (TRUE)
	{
		line = get_next_line(p[0]);
		if (!line)
			break ;
		temp = redir->heredoc_content;
		redir->heredoc_content = add_element_to_array(temp, &line,
				sizeof(char *));
		ft_free_node(1, temp);
	}
	close(p[0]);
	return (SUCCESS);
}

int	read_here_docs(t_exec_tree *tree, t_minishell *minishell)
{
	t_redir_info	**redir;

	if (!tree)
		return (0);
	read_here_docs(tree->left, minishell);
	read_here_docs(tree->right, minishell);
	redir = tree->redir;
	while (redir && *redir)
	{
		if ((*redir)->redir_type == HERE_DOC_REDI && !sigvar.exec_stop)
			save_heredoc_content(*redir, minishell);
		redir++;
	}
	if (sigvar.exec_stop)
		return (1);
	return (0);
}

t_stat	handle_heredoc(t_redir_info *redir, t_minishell *minishell,
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
