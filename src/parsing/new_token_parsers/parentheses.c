/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 15:10:42 by bamrouch          #+#    #+#             */
/*   Updated: 2023/05/16 16:41:31 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    make_parenthese_tokens(t_list *parenthese_node, t_minishell *new_mini)
{
    t_list	*new_tokens;
    char	*token;
    long    unclosed_parentheses;
    
	if (*(char *)parenthese_node->content == ')')
		exit_minishell(-1, "dont try closing something unopen )", TRUE);
    unclosed_parentheses = 1;
	parenthese_node = parenthese_node->next;
	new_tokens = NULL;
	while (unclosed_parentheses && parenthese_node)
    {
		token = parenthese_node->content;
		if (*token == '(')
			unclosed_parentheses++;
		else if (*token == ')')
			unclosed_parentheses--;
		if (unclosed_parentheses == 0)
			break;
		ft_lstadd_back(&new_tokens, pro_lstnew(parenthese_node->content));
        parenthese_node = parenthese_node->next;
    }
	if(parenthese_node)
		new_mini->n_parser_helper.post_logic_token = parenthese_node->next;
	else
		exit_minishell(-1, "need more of these )", TRUE);
    new_mini->tokens = new_tokens;
}

void    handle_parenthese(t_list *token_node, t_minishell *mini)
{
    char *token;
	t_minishell new_mini;
	t_exec_tree	*handled_parentheses;
	
	token = token_node->content;
	ft_bzero(&new_mini, sizeof(t_minishell));
	make_parenthese_tokens(token_node, &new_mini);
	mini->tokens = new_mini.n_parser_helper.post_logic_token;
	mini->n_parser_helper.prev_is_parenthese = TRUE;
	handled_parentheses = parsing_root(&new_mini);
	if(mini->exec_root->type)
	{
		handled_parentheses->parent = mini->exec_root;
		if(!mini->exec_root->left)
			mini->exec_root->left = handled_parentheses;
		else if(!mini->exec_root->right)
			mini->exec_root->right = handled_parentheses;
		else
			exit_minishell(-1, "come debug me", TRUE);
	}
	else
		mini->exec_root = handled_parentheses;
}
