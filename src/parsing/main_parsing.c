/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 21:47:35 by bamrouch          #+#    #+#             */
/*   Updated: 2023/05/17 17:18:25 by bamrouch         ###   ########.fr       */
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


void	 print_tree(t_exec_tree *node)
{
	printf("------------------------------\n");
	printf("node type : %d\n",node->type);
	if (node->left)
		printf("the left type |%d|\n", node->left->type);
	if (node->right)
		printf("the right type |%d|\n", node->right->type);
	if (node->type == 4)
	{
		while (*node->info.exec_node->cmd)
		{
			printf("command args :%s\n", *(node->info.exec_node->cmd));
			node->info.exec_node->cmd++;
		}
		while (node->info.exec_node->input && 
			node->info.exec_node->input->continue_redirs)
		{
			printf("input redirs found|%s|\n",node->info.exec_node->input->content);
			node->info.exec_node->input++;
		}
		while (node->info.exec_node->output && 
			node->info.exec_node->output->continue_redirs)
		{
			printf("output redirs found|%s|\n",node->info.exec_node->output->content);
			node->info.exec_node->output++;
		}
	}
	printf("------------------------------\n");
}

void	main_parsing(char *cmd, t_minishell *mini)
{
	
	tokenize(cmd, mini);
	strip_space_from_tokens(mini->tokens, mini);
	parsing_root(mini);
	loop_exec_tree(mini->exec_root, print_tree);
}

