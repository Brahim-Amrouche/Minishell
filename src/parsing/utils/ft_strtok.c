/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 20:12:35 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/03 02:37:13 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	char *mini_str_chr(char *str, t_boolean (*checker)(char c))
{
	if (!str)
		return NULL;
	while (*str)
	{
		if (checker(*str))
			return str;
		str++;
	}
	if(checker(*str))
		return str;
	return NULL;
}

char	*ft_strtok(char *str, t_boolean (*checker)(char c))
{
	static char *input;
	char		*token;

	if(str)
		input = str;
	if (!input)
		return NULL;
	while(*input && checker(*input))
		input++;
	token = input;
	input = mini_str_chr(input, checker);
	if (input)
	{
		*input = '\0';
		input++;
	}
	return token;
}
  
