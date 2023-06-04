/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 15:10:42 by bamrouch          #+#    #+#             */
/*   Updated: 2023/06/03 16:41:39 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parenthese_tokens_helper(t_list *parenthese_node,
		t_list **new_tokens, t_minishell *new_mini, long unclosed_parentheses)
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
	else if (!*new_tokens)
		exit_minishell(-1, "No empty parentheses", TRUE);
	else
		exit_minishell(-1, "need more of these )", TRUE);
}

void	make_parenthese_tokens(t_list *parenthese_node, t_minishell *new_mini)
{
	t_list	*new_tokens;
	long	unclosed_parentheses;

	if (*(char *)parenthese_node->content == ')')
		exit_minishell(-1, "dont try closing something unopen )", TRUE);
	unclosed_parentheses = 1;
	parenthese_node = parenthese_node->next;
	new_tokens = NULL;
	parenthese_tokens_helper(parenthese_node, &new_tokens, new_mini,
		unclosed_parentheses);
	new_mini->tokens = new_tokens;
}

void	handle_parenthese_node(t_exec_tree *handled_parentheses,
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
			exit_minishell(-1, "come debug me", TRUE);
	}
	else
		mini->exec_root = handled_parentheses;
}

void	handle_parenthese(t_list *token_node, t_minishell *mini)
{
	t_minishell	new_mini;
	t_exec_tree	*handled_parentheses;

	if (mini->exec_root->type == LOGICAL_EXEC)
		exit_minishell(-1, "No sush things before a parenthese", TRUE);
	ft_bzero(&new_mini, sizeof(t_minishell));
	make_parenthese_tokens(token_node, &new_mini);
	mini->tokens = new_mini.n_parser_helper.post_logic_token;
	handled_parentheses = parsing_root(&new_mini);
	handled_parentheses = exec_tree_node(3, LOGICAL_PARENTHESE,
			NULL, handled_parentheses);
	if (!handled_parentheses)
		exit_minishell(ENOMEM, "couldnt add LOGICAL PARENTHESE node", TRUE);
	mini->n_parser_helper.parenthese_node = handled_parentheses;
	handle_parenthese_node(handled_parentheses, mini);
}
