/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elasce <elasce@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 12:40:31 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/10 14:21:48 by elasce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mini_export(t_minishell *minishell, char *var)
{
	char	*cmd[3];

	cmd[0] = "export";
	cmd[2] = NULL;
	cmd[1] = var;
	export(minishell, cmd);
	return (0);
}

static char	*get_next_string_alphabetically(char **pool, char *drop)
{
	char	*next_drop;

	if (!pool)
		return (NULL);
	next_drop = "\255";
	while (*pool)
	{
		if (ft_strncmp(*pool, drop, -1) > 0 && ft_strncmp(*pool, next_drop,
				-1) < 0)
			next_drop = *pool;
		pool++;
	}
	return (next_drop);
}

int	print_export_data(t_minishell *minishell)
{
	char	**env_var;
	char	*var;
	int		i;
	int		j;
	int		fd;

	env_var = minishell->export_data;
	fd = 1;
	if (!env_var)
		return (0);
	i = 0;
	var = "\0";
	while (env_var[i])
	{
		var = get_next_string_alphabetically(env_var, var);
		j = 0;
		ft_putstr_fd("declare -x  ", fd);
		while (var[j] && var[j] != '=')
			ft_putchar_fd(var[j++], fd);
		if (var[j] == '=')
			printf("=\"%s\"", var + j + 1);
		printf("\n");
		i++;
	}
	return (0);
}

char	**add_or_replace_elem(char **arr, char *new_elem,
			char *var_name, t_boolean free)
{
	char	**old_elem;

	old_elem = get_env_var(var_name, arr);
	if (!new_elem)
		return (arr);
	if (!old_elem)
		return (add_elem_to_arr(arr, new_elem));
	if (free)
		ft_free_node(ENV_SCOPE, *old_elem);
	*old_elem = new_elem;
	return (arr);
}
