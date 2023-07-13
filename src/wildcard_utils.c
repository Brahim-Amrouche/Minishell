/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 12:17:29 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/13 01:58:48 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_stat	set_up_disection(size_t *i, size_t *k,
			size_t	***wildcard_index)
{
	*wildcard_index = (get_minishell(NULL))->n_parser_helper.wildcard_indexes;
	if (!(*wildcard_index))
		return (FAIL);
	*i = 0;
	*k = 0;
	return (SUCCESS);
}

static char	**disect_pattern(char *pattern, char **arr, char *elem, int *j)
{
	size_t	**wildcard_index;
	size_t	i;
	size_t	k;

	if (set_up_disection(&i, &k, &wildcard_index) == FAIL)
		return (add_elem_to_arr(NULL, pattern));
	while (pattern[i])
	{
		if (wildcard_index[k] && i == *(wildcard_index[k]))
		{
			if (i > 0)
			{
				elem = protected_substr(pattern, *j, i - *j);
				arr = add_element_to_array(arr, &elem, sizeof(char *));
			}
			while (wildcard_index[k] && i == *(wildcard_index[k]) && ++i && ++k)
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
