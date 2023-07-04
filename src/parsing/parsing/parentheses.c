/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 15:10:42 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/04 14:26:22 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	token_helper_error(t_list *new_tokens, t_minishell *new_mini)
{
	if (!*new_tokens)
		return (new_mini->parsing_err_code = return_msg(258,
				"minishell: syntax error empty ()"), (void)0);
	else
		return (new_mini->parsing_err_code = return_msg(258,
				"minishell: syntax error uneven number of ()"), (void)0);
}

static void	parenthese_tokens_helper(t_list *parenthese_node,
										t_list **new_tokens,
										t_minishell *new_mini,
										long unclosed_parentheses)
{
	char	*token;
	t_list	*new_node;

	while (unclosed_parentheses && parenthese_node)
	{
		token = parenthese_node->content;
		if (*token == '(')
			unclosed_parentheses++;
		else if (*token == ')')
			unclosed_parentheses--;
		if (unclosed_parentheses == 0)
			break ;
		new_node = pro_lstnew(parenthese_node->content);
		if (!new_node)
			exit_minishell(ENOMEM, "couldnt malloc a new parenthese token",
				TRUE);
		ft_lstadd_back(new_tokens, new_node);
		parenthese_node = parenthese_node->next;
	}
	if (parenthese_node)
		new_mini->n_parser_helper.post_logic_token = parenthese_node->next;
	else
		token_helper_error(*new_tokens, new_mini);
}

void	make_parenthese_tokens(t_list *parenthese_node, t_minishell *new_mini)
{
	t_list	*new_tokens;
	long	unclosed_parentheses;

	if (*(char *)parenthese_node->content == ')')
		return (new_mini->parsing_err_code = return_msg(258,
				"minishell: syntax error near unexpected token `)`"),
			(void)0);
	unclosed_parentheses = 1;
	parenthese_node = parenthese_node->next;
	new_tokens = NULL;
	parenthese_tokens_helper(parenthese_node, &new_tokens, new_mini,
		unclosed_parentheses);
	if (new_mini->parsing_err_code)
		return ;
	new_mini->tokens = new_tokens;
}

static void	handle_parenthese_node(t_exec_tree *handled_parentheses,
							t_minishell *mini)
{
	if (mini->exec_root->type)
	{
		handled_parentheses->parent = mini->exec_root;
		if (!mini->exec_root->left)
			mini->exec_root->left = handled_parentheses;
		else if (!mini->exec_root->right)
			mini->exec_root->right = handled_parentheses;
		else
			mini->parsing_err_code = return_msg(1,
					"mishell: tree left and right is full");
	}
	else
		mini->exec_root = handled_parentheses;
}

void	handle_parenthese(t_list *token_node, t_minishell *mini)
{
	t_minishell	new_mini;
	t_exec_tree	*handled_parentheses;

	if (mini->exec_root->type == LOGICAL_EXEC)
		return (mini->parsing_err_code = return_msg(258,
				"minishell: syntax error near unexpected token `()`"), (void)0);
	ft_bzero(&new_mini, sizeof(t_minishell));
	make_parenthese_tokens(token_node, &new_mini);
	if (new_mini.parsing_err_code)
		return (mini->parsing_err_code = new_mini.parsing_err_code, (void)0);
	mini->tokens = new_mini.n_parser_helper.post_logic_token;
	handled_parentheses = parsing_root(&new_mini);
	if (new_mini.parsing_err_code)
		return (mini->parsing_err_code = new_mini.parsing_err_code, (void)0);
	handled_parentheses = exec_tree_node(3, LOGICAL_PARENTHESE, NULL,
			handled_parentheses);
	if (!handled_parentheses)
		exit_minishell(ENOMEM, "couldnt add LOGICAL PARENTHESE node", TRUE);
	mini->n_parser_helper.parenthese_node = handled_parentheses;
	handle_parenthese_node(handled_parentheses, mini);
}
