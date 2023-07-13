/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:02:20 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/13 05:21:31 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**rm_arr_elem(char **arr, char **elem)
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
	ft_free_node(ENV_SCOPE, arr);
	return (new_arr);
}

static t_boolean	check_unset_syntax(char **args, int i, int *old_err)
{
	int	j;
	int	err;

	if (i == 1)
		*old_err = 0;
	j = 0;
	err = 0;
	if (args[i][j] != '_' && !ft_isalpha(args[i][j]))
		err = return_msg(1, "#unset: `$': not a valid identifier", args[i]);
	if (args[i][j])
		j++;
	while (args[i][j] && (ft_isalnum(args[i][j]) || args[i][j] == '_'))
		j++;
	if (!err && args[i][j] != '\0')
		err = return_msg(1, "#unset: `$': not a valid identifier", args[i]);
	if (err)
		*old_err = 1;
	if (err)
		return (FALSE);
	return (TRUE);
}

int	unset(t_minishell *minishell, char **args)
{
	char	**to_be_unset;
	int		i;
	char	*temp;
	int		err;

	i = 1;
	while (args[i])
	{
		temp = NULL;
		if (check_unset_syntax(args, i, &err))
		{
			to_be_unset = get_env_var(args[i], minishell->envp);
			if (to_be_unset)
				minishell->envp = rm_arr_elem(minishell->envp, to_be_unset);
			to_be_unset = get_env_var(args[i], minishell->export_data);
			if (to_be_unset)
				temp = *to_be_unset;
			if (to_be_unset)
				minishell->export_data = rm_arr_elem(minishell->export_data,
						to_be_unset);
			ft_free_node(ENV_SCOPE, temp);
		}
		i++;
	}
	return (err);
}
