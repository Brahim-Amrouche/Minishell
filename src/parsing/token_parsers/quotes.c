/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 02:56:12 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/12 04:07:18 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quotes_parser(t_list *tokens_list, t_minishell *mini)
{
	char    *token;
	char    to_find_token;
	size_t  i;
	size_t  j;

	token = tokens_list->content;
	i = 0;
	while (*(token + i) && *(token + i) != single_quote && *(token + i) != double_quote)
		i++;
	to_find_token = *(token + i);
	if (to_find_token)
	{
		j = i + 1;
		while (*(token + j) && *(token + j) != to_find_token)
			j++;
		if (!*(token + j))
			exit_minishell(-1, "unclosed token pattern", TRUE);
		tokens_list->content = replace_value_in_token(token, i, i + 1,  "");
		tokens_list->content = replace_value_in_token(tokens_list->content, j-1, j, "");
	}
	if (!to_find_token || to_find_token == double_quote)
		get_var(tokens_list, mini);
	token = tokens_list->content;
}