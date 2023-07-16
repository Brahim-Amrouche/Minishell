/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:27:20 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/16 00:35:10 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(char *arg, size_t *i, size_t j, t_boolean get_env)
{
	char		*removed_quotes;
	size_t		removed_quotes_len;
	char		*new_arg;
	t_minishell	*mini;

	mini = get_minishell(NULL);
	removed_quotes = protected_substr(arg, (*i) + 1, j - (*i) - 1);
	mini->n_parser_helper.remove_quotes = FALSE;
	if (get_env)
		removed_quotes = get_var(removed_quotes, mini, FALSE);
	mini->n_parser_helper.remove_quotes = TRUE;
	if (!removed_quotes)
		exit_minishell(ENOMEM, "could'nt remove quotes", TRUE);
	removed_quotes_len = ft_strlen(removed_quotes);
	new_arg = replace_value_in_arg(arg, *i, j + 1, removed_quotes);
	ft_free_node(1, arg);
	if (removed_quotes_len)
		*i += removed_quotes_len - 1;
	return (new_arg);
}

char	*unwrap_quotes(char *arg)
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
				arg = remove_quotes(arg, &i, j, FALSE);
		}
		i++;
	}
	return (arg);
}
