/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 23:46:46 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/11 16:48:04 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define TOKEN_SEPERATORS "(\n)\n&&\n||\n|\n<<\n>>\n<\n>\n \n\t\n"

static void	add_token_to_cmd(char *token, t_list **cmd_tokens)
{
	t_list	*new_node;

	new_node = pro_lstnew(token);
	if (!new_node)
		exit_minishell(ENOMEM, "couldn't malloc new_token node", TRUE);
	ft_lstadd_back(cmd_tokens, new_node);
}

void	tokenize(char *input, t_minishell *mini)
{
	char	*token;

	token = str_tokenize(input, TOKEN_SEPERATORS);
	if (!token)
		return ;
	while (token)
	{
		add_token_to_cmd(token, &mini->tokens);
		token = str_tokenize(NULL, TOKEN_SEPERATORS);
		if (!token)
			return ;
	}
}
