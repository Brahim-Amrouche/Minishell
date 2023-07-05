/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 12:17:29 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/05 12:56:48 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**disect_pattern(char *pattern, char **arr, char *elem, int *j)
{
	size_t	i;

	i = 0;
	while (pattern[i])
	{
		skip_quotes(pattern, &i);
		if (pattern[i] == '*')
		{
			if (i > 0)
			{
				elem = protected_substr(pattern, *j, i - *j);
				arr = add_element_to_array(arr, &elem, sizeof(char *));
			}
			while (pattern[++i] && pattern[i] == '*')
				;
			elem = pro_str_dup("*");
			arr = add_element_to_array(arr, &elem, sizeof(char *));
			*j = i;
			continue ;
		}
		i++;
	}
	return (arr);
}

char	**create_pattern_arr(char *pattern)
{
	char	**arr;
	char	*elem;
	int		j;

	arr = NULL;
	elem = NULL;
	j = 0;
	arr = disect_pattern(pattern, arr, elem, &j);
	if (*(pattern + j))
	{
		elem = pro_str_dup(pattern + j);
		arr = add_element_to_array(arr, &elem, sizeof(char *));
	}
	return (arr);
}
