/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:27:20 by bamrouch          #+#    #+#             */
/*   Updated: 2023/06/03 13:35:33 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(char *arg, t_minishell *mini, size_t *i, size_t j)
{
	char	*removed_quotes;
	size_t	removed_quotes_len;
	char	*new_arg;

	removed_quotes = protected_substr(arg, (*i) + 1, j - (*i) - 1);
	if (!removed_quotes)
		exit_minishell(ENOMEM, "could'nt remove quotes", TRUE);
	if (*(arg + (*i)) == DOUBLE_QUOTE)
		removed_quotes = get_var(removed_quotes, mini);
	removed_quotes_len = ft_strlen(removed_quotes);
	new_arg = replace_value_in_arg(arg, *i, j + 1, removed_quotes);
	ft_free_node(1, arg);
	*i += removed_quotes_len - 1;
	return (new_arg);
}

char	*unwrap_quotes(char *arg, t_minishell *mini)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (*(arg + i))
	{
		if (*(arg + i) == SINGLE_QUOTE || *(arg + i) == DOUBLE_QUOTE)
		{
			j = i + 1;
			while (*(arg + j) && *(arg + j) != *(arg + i))
				j++;
			if (*(arg + j) != '\0')
				arg = remove_quotes(arg, mini, &i, j);
		}
		i++;
	}
	return (arg);
}
