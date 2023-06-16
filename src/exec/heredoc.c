/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 14:13:35 by maboulkh          #+#    #+#             */
/*   Updated: 2023/06/16 16:36:16 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  open_heredoc(char	*limiter, int *p, t_minishell *minishell)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	minishell->n_parser_helper.arg_replacing = TRUE;
	limiter = unwrap_quotes(limiter);// bro check this
	minishell->n_parser_helper.arg_replacing = FALSE;
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
		if (match_str(line, limiter))
			break;
		write(p[1], line, ft_strlen(line));
		write(p[1], "\n", 1);
	}
	if (close(p[1]))
		print_msg(2, "minishell: $ (heredoc): can't be closed", limiter);
	exit(0);
}

static t_stat save_heredoc_content(t_redir_info *redir, t_minishell *minishell)
{
	// char	*limiter;
	char	*line;
	int		p[2];
	int		id;

	// limiter = redir->content;
	redir->has_quotes = has_quotes(redir->content);
	if (pipe(p) == -1)
		exit_minishell(1, "couldnt open pipe", TRUE);
	id = fork();
	if (!id)
		open_heredoc(redir->content, p, minishell);
	waitpid(id, minishell->stat, 0);
	if (close(p[1]))
		print_msg(2, "minishell: $: can't be closed", redir->content);
	redir->heredoc_content = NULL;
	while (TRUE)
	{
		line = get_next_line(p[0]);
		if (!line)
			break ;
		redir->heredoc_content = add_element_to_array(redir->heredoc_content,
									&line, sizeof(char *));
	}
	close(p[0]);
	return (SUCCESS);
}

// void open_here_doc(t_redir_info **redir, t_minishell *minishell)
// {
// 	while (redir && *redir)
// 	{
// 		if ((*redir)->redir_type == HERE_DOC_REDI)
// 			save_heredoc_content(*redir, minishell);
// 		redir++;
// 	}
// }

int read_here_docs(t_exec_tree *tree, t_minishell *minishell)
{
    t_redir_info **redir;

	if (!tree)
		return (0);
	read_here_docs(tree->left, minishell);
	read_here_docs(tree->right, minishell);
    redir = tree->redir;
    while (redir && *redir)
	{
		if ((*redir)->redir_type == HERE_DOC_REDI && !(*get_sigvar()).exec_stop)
			save_heredoc_content(*redir, minishell);
		redir++;
	}
	if ((*get_sigvar()).exec_stop)
		return (1);
	// open_here_doc(tree->redir, minishell);
	return (0);
}
