/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:02:20 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/03 18:53:06 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**rm_elem_from_arr(char **arr, char **elem)
{
	char	**new_arr;
	size_t	size;
	size_t	i;
	size_t	j;

	if (!elem || !arr)
		return (arr);
	size = 0;
	while (arr && arr[size])
		size++;
	new_arr = ft_malloc(size * sizeof(char *), m_info(NULL, ENV_SCOPE, NULL,
				0));
	i = 0;
	j = 0;
	while (arr && arr[j] && arr + j != elem)
		new_arr[i++] = arr[j++];
	if (arr + j == elem)
		j++;
	while (arr && arr[j])
		new_arr[i++] = arr[j++];
	new_arr[i] = NULL;
	ft_free_node(ENV_SCOPE, elem);
	return (new_arr);
}

int	unset(t_minishell *minishell, char **args)
{
	char	**to_be_unset;
	int		i;

	i = 1;
	while (args[i])
	{
		to_be_unset = get_env_var(args[i], minishell->envp);
		if (to_be_unset)
			minishell->envp = rm_elem_from_arr(minishell->envp, to_be_unset);
		to_be_unset = get_env_var(args[i], minishell->export_data);
		if (to_be_unset)
			minishell->export_data = rm_elem_from_arr(minishell->export_data,
					to_be_unset);
		i++;
	}
	return (0);
}
