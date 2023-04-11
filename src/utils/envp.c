/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:27:22 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/09 07:33:01 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

char **add_elem_to_arr(char **arr, char *new_elem)
{
	char	**new_arr;
	size_t	size;
	size_t	i;

	size = 0;
	while (arr && arr[size])
		size++;
	new_arr = ft_malloc((size + 2) *  sizeof(char *), m_info(NULL, ENV_SCOPE, NULL, 0));
	i = 0;
	while (arr && arr[i])
	{
		new_arr[i] = arr[i];
		i++;
	}
	new_arr[i++] = new_elem;
	mem_move(m_info(NULL, SUBSTR_SCOPE, new_elem, ENV_SCOPE));
	new_arr[i] = NULL;
	ft_free_node(ENV_SCOPE, arr);
	return (new_arr);
}

char	**copy_envp(char **envp)
{
	char **env;
	char *new;

	if (!envp)
		return (NULL);
	while (*envp)
	{
		new = protected_substr(*envp, 0, -1);
		env = add_elem_to_arr(env, new);
		envp++;
	}
	return (env);
}
