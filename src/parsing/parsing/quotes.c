/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 14:27:20 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/12 12:08:27 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*remove_quotes(char *arg, size_t *i, size_t j)
{
	char	*removed_quotes;
	size_t	removed_quotes_len;
	char	*new_arg;

	removed_quotes = protected_substr(arg, (*i) + 1, j - (*i) - 1);
	if (!removed_quotes)
		exit_minishell(ENOMEM, "could'nt remove quotes", TRUE);
	removed_quotes_len = ft_strlen(removed_quotes);
	new_arg = replace_value_in_arg(arg, *i, j + 1, removed_quotes);
	ft_free_node(1, arg);
	*i += removed_quotes_len - 1;
	return (new_arg);
}



// char	**unwrap_double_quotes(char *args, t_minishell *mini)
// {
// 	size_t	i;
// 	size_t	j;
// 	char	*quoted_arg;
// 	char	**arg_table;

// 	i = 0;
// 	while (args[i])
// 	{
// 		j = 0;
// 		if (args[i] == DOUBLE_QUOTE)
// 		{
// 			j = i + 1;
// 			while (args[j] && args[j] != DOUBLE_QUOTE)
// 				j++;
// 			quoted_arg = protected_substr(args, i + 1, j - i - 1);
// 			if (!quoted_arg)
// 				exit_minishell(ENOMEM, "couldnt unwrap double quotes", TRUE);
// 			quoted_arg = get_var(arg, mini, TRUE);
// 			args = replace_value_in_arg(args, i, j, quoted_arg);
// 			i += ft_strlen(quoted_arg) - 1;
// 		}
// 		else
// 		i++;
// 	}
// 	args = get_var(args, mini, TRUE);
// 	arg_table = add_element_to_array(NULL, &args,sizeof(char *));
// 	return (arg_table);
// }

char	**unwrap_double_quotes(char *args, t_minishell *mini)
{
	size_t	i;
	size_t	j;
	char	*quoted_arg;
	char	**arg_table;

	i = 0;
	while (args[i])
	{
		j = 0;
		if (args[i] == DOUBLE_QUOTE)
		{
			j = i + 1;
			while (args[j] && args[j] != DOUBLE_QUOTE)
				j++;
			quoted_arg = protected_substr(args, i + 1, j - i - 1);
			if (!quoted_arg)
				exit_minishell(ENOMEM, "couldnt unwrap double quotes", TRUE);
			quoted_arg = get_var(arg, mini, TRUE);
			args = replace_value_in_arg(args, i, j, quoted_arg);
			i += ft_strlen(quoted_arg) - 1;
		}
		else
		i++;
	}
	args = get_var(args, mini, TRUE);
	arg_table = add_element_to_array(NULL, &args,sizeof(char *));
	return (arg_table);
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
				arg = remove_quotes(arg, &i, j);
		}
		i++;
	}
	return (arg);
}
