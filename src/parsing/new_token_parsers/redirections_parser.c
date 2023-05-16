/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_parser.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 16:50:57 by bamrouch          #+#    #+#             */
/*   Updated: 2023/05/14 16:03:49 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	new_t_redirection(t_redirections *new_redir, char *content, int type)
{
	
	ft_bzero(new_redir, sizeof(t_redirections));
	new_redir->content = content;
	if (type == 1)
		new_redir->is_read = TRUE;
	else if (type == 2)
		new_redir->is_write = TRUE;
	else if (type == 3)
		new_redir->is_heredoc = TRUE;
	else if (type == 4)
		new_redir->is_append = TRUE;
	new_redir->continue_redirs = TRUE;
}

static	void	parse_here_doc_redir(t_list *redir_node, t_minishell *mini)
{
	char *content;
	t_redirections	new_heredoc;
	t_redirections	*old_redir_array;
	
	content = redir_node->content;
	if (!ft_strncmp(content, "<<", 2) || !ft_strncmp(content, ">>", 2) || *content == '<' || *content == '>')
		exit_minishell(-1, "no redirection after here_doc redir", TRUE);
	new_t_redirection(&new_heredoc ,content, 3);
	old_redir_array = mini->exec_root->info.exec_node->input;
	mini->exec_root->info.exec_node->input = add_element_to_array(old_redir_array, &new_heredoc, sizeof(t_redirections));
	if (!mini->exec_root->info.exec_node->input)
		exit_minishell(ENOMEM, "couldn't malloc a new input here_doc", TRUE);
	ft_free_node(1, old_redir_array);
}

static void	parse_append_redir(t_list *redir_node, t_minishell *mini)
{
	char *content;
	t_redirections new_append;
	t_redirections	*old_redir_array;

	content = redir_node->content;
	if (!ft_strncmp(content, "<<", 2) || !ft_strncmp(content, ">>", 2) || *content == '<' || *content == '>')
		exit_minishell(-1, "no redirection after append redir", TRUE);
	new_t_redirection(&new_append, content, 4);
	old_redir_array = mini->exec_root->info.exec_node->output;
	mini->exec_root->info.exec_node->output = add_element_to_array(old_redir_array, &new_append, sizeof(t_redirections));
	if (!mini->exec_root->info.exec_node->output)
		exit_minishell(ENOMEM, "couldn't malloc a new output append_redir", TRUE);
	ft_free_node(1, old_redir_array);
}


static	void parse_write_redir(t_list *redir_node, t_minishell *mini)
{
	char	*content;
	t_redirections	new_write;
	t_redirections	*old_redir_array;

	content = redir_node->content;
	if (!ft_strncmp(content, "<<", 2) || !ft_strncmp(content, ">>", 2) || *content == '<')
		exit_minishell(-1, "no redirection after write redir", TRUE);
	new_t_redirection(&new_write, content, 2);
	old_redir_array = mini->exec_root->info.exec_node->output;
	mini->exec_root->info.exec_node->output = add_element_to_array(old_redir_array, &new_write, sizeof(t_redirections));
	if(!mini->exec_root->info.exec_node->output)
		exit_minishell(ENOMEM, "couldn't malloc new input write_redir", TRUE);
	ft_free_node(1, old_redir_array);
}

static void parse_read_redir(t_list *redir_node, t_minishell *mini)
{
	char *content;
	t_redirections new_read;
	t_redirections	*old_redir_array;

	content = redir_node->content;
	if (!ft_strncmp(content, "<<", 2) || !ft_strncmp(content, ">>", 2))
		exit_minishell(-1, "no redirection after read redir", TRUE);
	else if (*content == '>')
	{
		redir_node = redir_node->next;
		parse_write_redir(redir_node, mini);
		content = redir_node->content;
	}
	new_t_redirection(&new_read, content, 1);
	old_redir_array = mini->exec_root->info.exec_node->input;
	mini->exec_root->info.exec_node->input = add_element_to_array(old_redir_array, &new_read, sizeof(t_redirections));
	if(!mini->exec_root->info.exec_node->input)
		exit_minishell(ENOMEM, "couldn't malloc new input read_redir", TRUE);
	ft_free_node(1, old_redir_array);
}

void    parse_redirections(t_list *redir_node, t_minishell *mini)
{
    char *content;

	if(!redir_node->next)
		exit_minishell(-1, "need a file path after redir call", TRUE);
    content = redir_node->content;
    if (!ft_strncmp(content, "<<", 2))
        parse_here_doc_redir(redir_node->next, mini);
    else if (!ft_strncmp(content, ">>", 2))
        parse_append_redir(redir_node->next, mini);
	else if (*content == '<')
		parse_read_redir(redir_node->next, mini);
	else if (*content == '>')
		parse_write_redir(redir_node->next, mini);
}
