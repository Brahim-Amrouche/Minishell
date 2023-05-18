/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 21:47:35 by bamrouch          #+#    #+#             */
/*   Updated: 2023/05/18 20:50:28 by maboulkh         ###   ########.fr       */
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


void	 print_tree(t_exec_tree *node, t_minishell *mini)
{
	char **cmd;
	t_redirections *input, *output;

	printf("------------------------------\n");
	printf("node type : %d\n",node->type);
	if (node->left)
		printf("the left type |%d|\n", node->left->type);
	if (node->right)
		printf("the right type |%d|\n", node->right->type);
	if (node->type == 4)
	{
		cmd = node->info.exec_node->cmd;
		while (*cmd)
		{
			*cmd = unwrap_quotes(*cmd, mini);
			printf("command args :%s\n", *cmd);
			cmd++;
		}
		input = node->info.exec_node->input;
		while (input && input->continue_redirs)
		{
			printf("input redirs found|%s|\n",input->content);
			input++;
		}
		output = node->info.exec_node->output;
		while (output && output->continue_redirs)
		{
			printf("output redirs found|%s|\n",output->content);
			output++;
		}
	}
	printf("------------------------------\n");
}

void	main_parsing(char *cmd, t_minishell *mini)
{
	
	tokenize(cmd, mini);
	strip_space_from_tokens(mini->tokens, mini);
	parsing_root(mini);
	// loop_exec_tree(mini->exec_root, print_tree);
}

