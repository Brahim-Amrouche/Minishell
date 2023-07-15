/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elasce <elasce@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 12:17:29 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/15 03:44:57 by elasce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**disect_pattern(char *pattern, char *map, char *elem, int *j)
{
	char	**arr;
	size_t	i;

	arr = NULL;
	i = 0;
	while (pattern[i])
	{
		if (pattern[i] == '*' && map[i] == '*')
		{
			if (i > 0)
			{
				elem = protected_substr(pattern, *j, i - *j);
				arr = add_element_to_array(arr, &elem, sizeof(char *));
			}
			while (pattern[++i] && pattern[i] == '*' && map[i] == '*')
				;
			elem = pro_str_dup("");
			arr = add_element_to_array(arr, &elem, sizeof(char *));
			*j = i;
			continue ;
		}
		i++;
	}
	return (arr);
}

char	**create_pattern_arr(char *pattern, char *map)
{
	char	**arr;
	char	*elem;
	int		j;

	elem = NULL;
	j = 0;
	arr = disect_pattern(pattern, map, elem, &j);
	if (*(pattern + j))
	{
		elem = pro_str_dup(pattern + j);
		arr = add_element_to_array(arr, &elem, sizeof(char *));
	}
	return (arr);
}
