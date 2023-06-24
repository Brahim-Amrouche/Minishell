/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:02:20 by maboulkh          #+#    #+#             */
/*   Updated: 2023/06/22 18:36:11 by maboulkh         ###   ########.fr       */
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

int	unset(t_minishell *minishell, char **args, int index)
{
	char **to_be_unset;
	char ***export_data;
	char *arg;

	arg = *(args + (++index));
	if (!arg)
		return (0);
	to_be_unset = get_env_var(arg, minishell->envp);
	if (to_be_unset)
		minishell->envp = rm_elem_from_arr(minishell->envp, to_be_unset);
	export_data = fetch_export_data();
	to_be_unset = get_env_var(arg, *export_data);
	if (to_be_unset)
		*export_data = rm_elem_from_arr(*export_data, to_be_unset);
	// what about export_data
	unset(minishell, args, index);
	return (0);
}