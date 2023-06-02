/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 21:47:35 by bamrouch          #+#    #+#             */
/*   Updated: 2023/06/02 13:47:04 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	token_space_striper_helper(t_list *root,
		t_list *tokens, t_minishell *mini)
{
	char	*content;
	t_list	*prev;

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
	token_space_striper_helper(root, tokens, mini);
}

void	print_tree(t_exec_tree *node, t_minishell *mini)
{
	char	**cmd;
	t_redir_info **redir;

	printf("------------------------------\n");
	printf("node type : %d\n", node->type);
	if (node->left)
		printf("the left type |%d|\n", node->left->type);
	if (node->right)
		printf("the right type |%d|\n", node->right->type);
	if (node->type == 4 || node->type == 5)
	{
		cmd = node->argv;
		while (cmd && *cmd)
		{
			*cmd = unwrap_quotes(*cmd, mini);
			printf("command args :%s\n", *cmd);
			cmd++;
		}
	}
	redir = node->redir;
	while (redir && *redir)
	{
		printf("redir :%s, type = %d , level = %d\n", (*redir)->content, (*redir)->redir_type, (*redir)->level);
		redir++;
	}
printf("------------------------------\n");
}

void	main_parsing(char *cmd, t_minishell *mini)
{
tokenize(cmd, mini);
strip_space_from_tokens(mini->tokens, mini);
parsing_root(mini);
loop_exec_tree(mini->exec_root, mini, print_tree);
}
