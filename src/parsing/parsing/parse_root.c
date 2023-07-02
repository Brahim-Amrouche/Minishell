/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_root.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 14:39:21 by bamrouch          #+#    #+#             */
/*   Updated: 2023/06/24 14:05:14 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_args(char *token, t_minishell *mini)
{
	char		**temp;
	t_exec_tree	*exec_node;

	exec_node = mini->exec_root;
	if (exec_node->type == LOGICAL_NONE || (exec_node->type == LOGICAL_EXEC
			&& mini->n_parser_helper.parenthese_node == NULL))
		exec_node->type = LOGICAL_EXEC;
	else
		exit_minishell(-1, "dont give me args like this", TRUE);
	temp = exec_node->argv;
	exec_node->argv = add_element_to_array(temp, &token, sizeof(char *));
	if (!exec_node->argv)
		exit_minishell(ENOMEM, "couldn't add a new arg to cmds array", TRUE);
	ft_free_node(1, temp);
	mini->tokens = mini->tokens->next;
}

t_exec_tree	*parsing_root(t_minishell *mini)
{
	char	*token_content;

	mini->exec_root = exec_tree_node(1, LOGICAL_NONE);
	if (!mini->exec_root)
		exit_minishell(ENOMEM, "couldn't create a new node", TRUE);
	while (mini->tokens && !mini->parsing_err_code)
	{
		token_content = mini->tokens->content;
		if (*token_content == '(' || *token_content == ')')
			handle_parenthese(mini->tokens, mini);
		else if (!ft_strncmp(token_content, "||", 2)
			|| !ft_strncmp(token_content, "&&", 2) || *token_content == '|')
			parse_logical_operators(mini->tokens, mini, token_content);
		else if (!ft_strncmp(token_content, "<<", 2)
			|| !ft_strncmp(token_content, ">>", 2) || *token_content == '<'
			|| *token_content == '>')
			parse_redirections(mini->tokens, mini);
		else
			handle_args(token_content, mini);
	}
	return (mini->exec_root);
}
