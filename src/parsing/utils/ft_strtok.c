/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 20:12:35 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/04 06:13:48 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_boolean ft_is_space(char c)
{
    if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ')
        return TRUE;
    return FALSE;
}

t_tokenizer forced_token(t_boolean read, ...)
{
	va_list args;   
	static t_tokenizer forced_token;

	va_start(args, read);
    if (read)
        return forced_token;
    else
        forced_token = va_arg(args, t_tokenizer);
	va_end(args);
    return forced_token;
}

static	size_t mini_str_chr(char *str, t_boolean (*checker)(char *))
{
	size_t			i;
	t_tokenizer		f_token;

	i = 0;
	if (!str)
		return i;
	while (*(str + i))
	{
		if (checker((str + i)))
		{
			f_token = forced_token(TRUE);
			i += f_token.step;
			if (f_token.ignore)
				return i;
			if (f_token.allows_space)
				while(*(str + i) && ft_is_space(*(str + i)))
					i++;
			while(*(str + i) && f_token.break_token && *(str + i) != f_token.break_token)
				i++;
			while (*(str + i) && !ft_is_space(*(str + i)))
				i++;
			return i;
		}
		i++;
	}
	return i;
}


char	*ft_strtok(char *str, t_boolean (*checker)(char *))
{
	static char *input;
	char		*token;
	size_t		token_pos;

	if(str)
		input = str;
	if (!input)
		return NULL;
	while(*input && ft_is_space(*input))
		input++;
	token_pos = mini_str_chr(input, checker);
	if (*(input + token_pos))
	{
		token = protected_substr(input, 0, token_pos);
		input += token_pos;
		if (!*input)
			input = NULL;
	}
	else
	{
		token = input;
		input = NULL;
	}
	return token;
}
  
