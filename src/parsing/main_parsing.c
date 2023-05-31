/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 21:47:35 by bamrouch          #+#    #+#             */
/*   Updated: 2023/05/31 21:06:15 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	token_space_striper_helper(t_list *prev, t_list *root,
		t_list *tokens, t_minishell *mini)
{
	char	*content;

	prev = root;
	tokens = tokens->next;
	while (tokens)
	{
		content = tokens->content;
		if (*content == ' ')
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

static void	strip_space_from_tokens(t_list *tokens, t_minishell *mini)
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
			break ;
		}
		tokens = tokens->next;
	}
	token_space_striper_helper(prev, root, tokens, mini);
}

// void	print_tree(t_exec_tree *node, t_minishell *mini)
// {
// 	char	**cmd;

// 	printf("------------------------------\n");
// 	printf("node type : %d\n", node->type);
// 	if (node->left)
// 		printf("the left type |%d|\n", node->left->type);
// 	if (node->right)
// 		printf("the right type |%d|\n", node->right->type);
// 	if (node->type == 4 || node->type == 5)
// 	{
// 		cmd = node->info.content;
// 		while (*cmd)
// 		{
// 			*cmd = unwrap_quotes(*cmd, mini);
// 			printf("command args :%s\n", *cmd);
// 			if (node->type == 4 && node->info.low_prio_redir)
// 			{
// 				printf("is low prio\n");
// 			}
// 			cmd++;
// 		}
// 	}
// 	cmd = node->redir.content;
// 	while (cmd && *cmd)
// 	{
// 		printf("redir :%s\n", *cmd);
// 		cmd++;
// 	}
// 	printf("------------------------------\n");
// }

void	main_parsing(char *cmd, t_minishell *mini)
{
	tokenize(cmd, mini);
	strip_space_from_tokens(mini->tokens, mini);
	parsing_root(mini);
	// loop_exec_tree(mini->exec_root, mini, print_tree);
}
