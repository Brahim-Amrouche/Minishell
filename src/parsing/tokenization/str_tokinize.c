/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_tokinize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 00:47:00 by bamrouch          #+#    #+#             */
/*   Updated: 2023/05/27 15:18:00 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define DOUBLE_QUOTE_SEP "\042\n"
#define UNIQUE_QUOTE_SEP "\047\n"

static int	check_if_sep(char *s, char *seprators)
{
	size_t j;
	size_t i;
	
	j = 0;
	while (seprators[j])
	{
		i = 0;
		while (seprators[j] && seprators[j] != '\n' && s[i])
		{
			if(seprators[j] == s[i])
			{
				i++;
				j++;
			}
			else
				break;
		}
		if (!seprators || seprators[j] == '\n')
			return i;
		else
		{
			while (seprators[j] && seprators[j] != '\n')
				j++;
			if (seprators[j])
				j++;
		}
	}
	return -1;
}

static size_t index_str_chr(char *s, char *seprators)
{
	size_t	i;
	int		token_index;
	char	*cancel_token;

	i = 0;
	cancel_token = NULL;
	while (s[i])
	{
		if (!cancel_token && (check_if_sep(s + i, DOUBLE_QUOTE_SEP) >= 0 || check_if_sep(s + i, UNIQUE_QUOTE_SEP) >= 0))
		{
			if(s[i] == '\042')
				cancel_token = DOUBLE_QUOTE_SEP;
			else
				cancel_token = UNIQUE_QUOTE_SEP;
		}
		else if (cancel_token && check_if_sep(s + i, cancel_token) >= 0)
			cancel_token = NULL;
		else if (!cancel_token)
		{ 
			token_index = check_if_sep(s + i, seprators);
			if (token_index >= 0)
			{
				if (i == 0)
					i += token_index;
				break;
			}
		}
		i++;
	}
	if (cancel_token)
		exit_minishell(-1, "unclosed quotes", TRUE);
	return i;
}

char	*str_tokenize(char *str, char *seperators)
{
	static char	*input;
	char		*token;
	size_t		token_pos;

	if (str)
		input = str;
	if (!input)
		return NULL;
	token_pos = index_str_chr(input, seperators);
	if (*(input + token_pos))
	{
		if (!token_pos)
			token_pos++;
		token = protected_substr(input, 0, token_pos);
		if (!token)
			exit_minishell(ENOMEM, "could't malloc token node", TRUE);
		input += token_pos;
		if (!*input)
			input = NULL;
	}
	else
	{
		token = protected_substr(input, 0, ft_strlen(input));
		if (!token)
			exit_minishell(ENOMEM, "could't malloc token node", TRUE);
		input = NULL;
	}
	return (token);
}