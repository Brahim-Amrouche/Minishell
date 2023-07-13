/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 02:00:48 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/13 06:59:22 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_boolean	is_dollar_char(char c)
{
	if (ft_isalpha(c) || c == '_')
		return (TRUE);
	return (FALSE);
}

static void	add_wildcard_index(t_minishell *mini, size_t i)
{
	size_t	*wildcard_i;
	size_t	**temp;

	wildcard_i = ft_malloc(sizeof(size_t), m_info(NULL, 1, NULL, 0));
	*wildcard_i = i;
	temp = mini->n_parser_helper.wildcard_indexes;
	mini->n_parser_helper.wildcard_indexes = add_element_to_array(temp,
			&wildcard_i, sizeof(size_t *));
	ft_free_node(1, temp);
}

t_boolean	check_if_var(char *arg, size_t i, size_t j)
{
	if (arg[i] == DOLLAR_SIGN
		&& !ft_isdigit(arg[j]) && arg[j] != '\"' && arg[j] != '\''
		&& arg[j] != '?' && is_dollar_char(arg[j]))
		return (TRUE);
	return (FALSE);
}

t_boolean	check_if_special_var(char *arg, size_t i, size_t j)
{
	if (arg[i] == DOLLAR_SIGN && (ft_isdigit(arg[j])
			|| arg[j] == '?' || arg[j] == '\"' || arg[j] == '\''))
		return (TRUE);
	return (FALSE);
}

void	get_wildcard_indexes(char *args, t_minishell *mini)
{
	size_t	i;
	size_t	j;
	size_t	**quotes_indexes;
	size_t	len;

	mini->n_parser_helper.wildcard_indexes = NULL;
	quotes_indexes = mini->n_parser_helper.quote_indexes;
	i = 0;
	j = 0;
	len = ft_strlen(args);
	while (i < len && args && args[i])
	{
		if (quotes_indexes && quotes_indexes[j] && i == quotes_indexes[j][0])
		{
			if (quotes_indexes[j][1] == quotes_indexes[j][0])
				(quotes_indexes[j][1])++;
			i = quotes_indexes[j++][1];
			continue ;
		}
		else if (args[i] == '*')
			add_wildcard_index(mini, i);
		i++;
	}
}
