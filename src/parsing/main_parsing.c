/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 21:47:35 by bamrouch          #+#    #+#             */
/*   Updated: 2023/05/15 13:54:47 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	strip_space_from_tokens(t_list *tokens, t_minishell *mini)
{
	t_list	*prev;
	char	*content;
	t_list	*root;

	root = NULL;
	while (tokens)
	{
		content = tokens->content;
		if (*content != ' ')
		{
			root = tokens;
			break;
		}
		tokens = tokens->next;
	}
	prev = root;
	tokens = tokens->next;
	while(tokens)
	{
		content = tokens->content;
		if(*content == ' ')
			;
		else
		{
			prev->next = tokens;
			prev = tokens;
		}
		tokens = tokens->next;
	}
	prev->next = NULL;
	mini->tokens = root;
}

void	main_parsing(char *cmd, t_minishell *mini)
{
	
	tokenize(cmd, mini);
	strip_space_from_tokens(mini->tokens, mini);
	// while (mini->tokens)
	// {
	// 	printf("|%s|\n", (char *)mini->tokens->content);
	// 	mini->tokens = mini->tokens->next;
	// }
	parsing_root(mini);
	while (mini->exec_root)
	{
		printf("node type : %d\n",mini->exec_root->type);
		if (mini->exec_root->left)
			printf("the left type |%d|\n", mini->exec_root->left->type);
		if (mini->exec_root->right)
			printf("the right type |%d|\n", mini->exec_root->right->type);
		if (mini->exec_root->type == 4)
		{
			while (*mini->exec_root->info.exec_node->cmd)
			{
				printf("command args :%s\n", *(mini->exec_root->info.exec_node->cmd));
				mini->exec_root->info.exec_node->cmd++;
			}
		}
		mini->exec_root = mini->exec_root->left;
	}
	// parse_tokens(mini);
}

