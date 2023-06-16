/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 14:40:58 by bamrouch          #+#    #+#             */
/*   Updated: 2023/06/16 16:29:30 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_quotes(char *arg, size_t *i)
{
	char	quote_type;
	int		back_up;

	back_up = *i;
	if (*(arg + *i) == DOUBLE_QUOTE || *(arg + *i) == SINGLE_QUOTE)
	{
		quote_type = *(arg + *i);
		(*i)++;
		while (*(arg + *i) && *(arg + *i) != quote_type)
			(*i)++;
		if (*(arg + *i) == '\0')
			*i = back_up;
	}
}
