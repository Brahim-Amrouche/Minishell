/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_operators.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 19:06:57 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/04 13:24:46 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	logical_tokens_helper(t_list *post_logic_node,
	t_minishell *new_mini,
	t_boolean is_pipe)
{
	t_list	*new_tokens;
	t_list	*new_node;
	char	*content;

	new_tokens = NULL;
	while (post_logic_node)
	{
		content = post_logic_node->content;
		if (!ft_strncmp(content, "&&", 2) || !ft_strncmp(content, "||", 2)
			|| (is_pipe && *content == '|'))
			break ;
		new_node = pro_lstnew(post_logic_node->content);
		if (!new_node)
			exit_minishell(ENOMEM,
				"couldn't malloc a new logical token node",
				TRUE);
		ft_lstadd_back(&new_tokens, new_node);
		post_logic_node = post_logic_node->next;
	}
	new_mini->n_parser_helper.post_logic_token = post_logic_node;
	new_mini->tokens = new_tokens;
}

static void	make_logical_tokens(t_list *post_logic_node, t_minishell *new_mini,
		t_boolean is_pipe)
{
	char	*content;

	if (!post_logic_node)
		return (new_mini->parsing_err_code = return_msg(258,
				"minishell: logical command needs another command"), (void) 0);
	content = post_logic_node->content;
	if (*content == '(' || *content == ')')
		make_parenthese_tokens(post_logic_node, new_mini);
	else
		logical_tokens_helper(post_logic_node, new_mini, is_pipe);
}

static void	make_new_root(char *token_content, t_minishell *mini,
	t_minishell *new_mini, t_exec_tree **new_root)
{
	if (!ft_strncmp(token_content, "&&", 2))
		*new_root = exec_tree_node(4, LOGICAL_AND, NULL, mini->exec_root,
				parsing_root(new_mini));
	else if (!ft_strncmp(token_content, "||", 2))
		*new_root = exec_tree_node(4, LOGICAL_OR, NULL, mini->exec_root,
				parsing_root(new_mini));
	else
		*new_root = exec_tree_node(4, LOGICAL_PIPE, NULL, mini->exec_root,
				parsing_root(new_mini));
}

void	parse_logical_operators(t_list *logical_node, t_minishell *mini,
		char *token_content)
{
	t_exec_tree	*new_root;
	t_minishell	new_mini;
	size_t		token_len;

	ft_bzero(&new_mini, sizeof(t_minishell));
	token_len = ft_strlen(token_content);
	if (mini->exec_root->type)
	{
		make_logical_tokens(logical_node->next, &new_mini, token_len < 2);
		if (new_mini.parsing_err_code)
			return (mini->parsing_err_code = new_mini.parsing_err_code,
				(void) 0);
		mini->tokens = new_mini.n_parser_helper.post_logic_token;
		make_new_root(token_content, mini, &new_mini, &new_root);
		if (!new_root)
			exit_minishell(ENOMEM, "couldn't malloc a new pipe", TRUE);
		if (new_mini.parsing_err_code)
			return (mini->parsing_err_code = new_mini.parsing_err_code,
				(void) 0);
		mini->exec_root->parent = new_root;
		mini->exec_root = new_root;
	}
	else
		mini->parsing_err_code = return_msg(258,
				"minishell: wrong usage of logical operators");
}
