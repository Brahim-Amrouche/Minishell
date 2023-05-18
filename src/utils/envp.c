/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:27:22 by maboulkh          #+#    #+#             */
/*   Updated: 2023/05/18 20:39:05 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

char **add_elem_to_arr(char **arr, char *new_elem)
{
	char	**new_arr;
	size_t	size;
	size_t	i;

	if (!new_elem)
		return (arr);
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

char	**export_envp(t_minishell *minishell, char **envp)
{
	char *cmd[3];
	t_exec_node node;

	node.cmd = (char **) &cmd;
	cmd[0] = "export";
	cmd[2] = NULL;
	if (!envp)
		return (NULL);
	while (*envp)
	{
		cmd[1] = *envp;
		export(minishell, &node, 0);
		envp++;
	}
	return (minishell->envp);
}
