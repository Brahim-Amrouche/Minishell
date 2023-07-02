/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:02:11 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/02 23:30:52 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**add_or_replace_elem(char **arr, char *new_elem, char *var_name)
{
	char	**old_elem;

	old_elem = get_env_var(var_name, arr);
	if (!new_elem)
		return (arr);
	if (!old_elem)
		return (add_elem_to_arr(arr, new_elem));
	*old_elem = new_elem;
	return (arr);
}

static t_export	check_export_type(char *token)
{
	if (!token)
		return (ERROR);
	if (*token != '_' && !ft_isalpha(*token++))
		return (ERROR);
	while (*token && (ft_isalnum(*token) || *token == '_'))
		token++;
	if (!*token)
		return (DECLARE);
	else if (*token == '=')
		return (ASSIGN);
	else if (*token == '+' && *(token + 1) == '=')
		return (APPEND);
	return (ERROR);
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

static int	print_export_data(void)
{
	char	**env_var;
	char	*var;
	int		i;
	int		j;
	int		fd;

	env_var = *fetch_export_data();
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

char	*get_export_variable_name(char *var, t_export type)
{
	size_t	len;

	if (!var)
		return (NULL);
	if (type == ERROR)
		return (protected_substr(var, 0, -1));
	len = 0;
	while (var[len] && var[len] != '=' && var[len] != '+')
		len++;
	return (protected_substr(var, 0, len));
}

char	*export_append(char **old_var_ptr, char *var, t_export type)
{
	char	*old_var;

	old_var = NULL;
	if (old_var_ptr)
		old_var = *old_var_ptr;
	if (!var)
		return (NULL);
	if (type == ASSIGN || type == DECLARE)
		return (pro_strjoin(NULL, var));
	else if (type == APPEND)
	{
		while (*var && *var != '=')
			var++;
		if (*var == '=' && ft_strchr(old_var, '='))
			return (pro_strjoin(old_var, var + 1));
		else if (*var == '=' && !ft_strchr(old_var, '='))
			return (pro_strjoin(old_var, var));
	}
	return (NULL);
}

char	***fetch_export_data(void)
{
	static char	**export_data;

	return (&export_data);
}

static void	exporting(t_minishell *minishell, char *arg, char *var_name,
		t_export export_type)
{
	char	***export_data;
	char	**existing_var;
	char	*var;

	export_data = fetch_export_data();
	existing_var = get_env_var(var_name, *export_data);
	var = export_append(existing_var, arg, export_type);
	if (export_type == DECLARE && existing_var)
		return ;
	if (ft_strncmp(var, "_=", 2))
		*export_data = add_or_replace_elem(*export_data, var, var_name);
	if (export_type != DECLARE)
		minishell->envp = add_or_replace_elem(minishell->envp, var, var_name);
}

int	export(t_minishell *minishell, char **args, int index)
{
	char		*var_name;
	char		*arg;
	t_export	export_type;

	arg = *(args + (++index));
	if (!arg)
		return (print_export_data());
	export_type = check_export_type(arg);
	var_name = get_export_variable_name(arg, export_type);
	if (export_type)
		exporting(minishell, arg, var_name, export_type);
	else
	{
		minishell->cmd_status = 1;
		print_msg(2, "minishell: export: `$': not a valid identifier",
				var_name);
	}
	ft_free_node(SUBSTR_SCOPE, var_name);
	if (*(args + (index + 1)))
		export(minishell, args, index);
	//  waht about in case of error
	return (0);
}

int mini_export(t_minishell *minishell, char *var)
{
	char	*cmd[3];

	cmd[0] = "export";
	cmd[2] = NULL;
	cmd[1] = var;
	export(minishell, cmd, 0);
	return (0);
}
