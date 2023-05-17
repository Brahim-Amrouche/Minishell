/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_parser.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 16:50:57 by bamrouch          #+#    #+#             */
/*   Updated: 2023/05/17 19:43:15 by bamrouch         ###   ########.fr       */
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
	mini->tokens = redir_node->next;
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
	mini->tokens = redir_node->next;
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
	mini->tokens = redir_node->next;
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
	mini->tokens = redir_node->next;
}


void	fill_parenthese_redirection(t_list *redir_node,t_exec_tree *new_root, t_minishell *mini,void (*f_redir)(t_list *redir_node, t_minishell *mini))
{
	if (!new_root)
		return ;
	fill_parenthese_redirection(redir_node, new_root->left, mini, f_redir);
	fill_parenthese_redirection(redir_node, new_root->right, mini, f_redir);
	if (new_root->type == 4)
	{
		mini->exec_root = new_root;
		f_redir(redir_node, mini);
	}
}

void    parse_redirections(t_list *redir_node, t_minishell *mini)
{
    char *content;
	void (*redir)(t_list *redir_node, t_minishell *mini);
	t_exec_tree *root;
	
	if(!redir_node->next)
		exit_minishell(-1, "need a file path after redir call", TRUE);
	content = redir_node->content;
    if (!ft_strncmp(content, "<<", 2))
        redir = parse_here_doc_redir;
    else if (!ft_strncmp(content, ">>", 2))
        redir = parse_append_redir;
	else if (*content == '<')
		redir = parse_read_redir;
	else if (*content == '>')
		redir = parse_write_redir;
	if (mini->n_parser_helper.parenthese_node)
	{
		root = mini->exec_root;
		fill_parenthese_redirection(redir_node->next, mini->n_parser_helper.parenthese_node, mini, redir);
		mini->exec_root = root;
	}
	else
		redir(redir_node->next, mini);
}
