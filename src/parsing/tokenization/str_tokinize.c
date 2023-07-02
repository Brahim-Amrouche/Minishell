/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_tokinize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 00:47:00 by bamrouch          #+#    #+#             */
/*   Updated: 2023/06/24 21:25:33 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define DOUBLE_QUOTE_SEP "\042\n"
#define UNIQUE_QUOTE_SEP "\047\n"

static t_boolean	found_a_token(char *s, char *seprators, size_t *i,
		char **cancel_token)
{
	int	token_index;

	if (!*cancel_token && (check_if_sep(s + *i, DOUBLE_QUOTE_SEP) >= 0
			|| check_if_sep(s + *i, UNIQUE_QUOTE_SEP) >= 0))
	{
		if (s[*i] == '\042')
			*cancel_token = DOUBLE_QUOTE_SEP;
		else
			*cancel_token = UNIQUE_QUOTE_SEP;
	}
	else if (*cancel_token && check_if_sep(s + *i, *cancel_token) >= 0)
		*cancel_token = NULL;
	else if (!*cancel_token)
	{
		token_index = check_if_sep(s + *i, seprators);
		if (token_index >= 0)
		{
			if (*i == 0)
				*i += token_index;
			return (TRUE);
		}
	}
	(*i)++;
	return (FALSE);
}

#define UNCLOSED_QUOTES "minishell: unclosed quotes"

static size_t	index_str_chr(char *s, char *seprators)
{
	size_t	i;
	char	*cancel_token;

	i = 0;
	cancel_token = NULL;
	while (s[i] && !found_a_token(s, seprators, &i, &cancel_token))
		continue ;
	if (cancel_token)
		get_minishell(NULL)->parsing_err_code = return_msg(258, UNCLOSED_QUOTES);
	return (i);
}

static void	str_tokenize_helper(char **input, size_t token_pos, char **token)
{
	if (!token_pos)
		token_pos++;
	*token = protected_substr(*input, 0, token_pos);
	if (!*token)
		exit_minishell(ENOMEM, "could't malloc token node", TRUE);
	*input += token_pos;
	if (!**input)
		*input = NULL;
}

char	*str_tokenize(char *str, char *seperators)
{
	static char	*input;
	char		*token;
	size_t		token_pos;

	if (str)
		input = str;
	if (!input)
		return (NULL);
	token_pos = index_str_chr(input, seperators);
	if (get_minishell(NULL)->parsing_err_code)
		return NULL;
	if (*(input + token_pos))
		str_tokenize_helper(&input, token_pos, &token);
	else
	{
		token = protected_substr(input, 0, ft_strlen(input));
		if (!token)
			exit_minishell(ENOMEM, "could't malloc token node", TRUE);
		input = NULL;
	}
	return (token);
}
