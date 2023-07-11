/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:02:11 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/11 22:42:33 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	size_t	i;

	i = 0;
	if (!var)
		return (NULL);
	if (type == ASSIGN || type == DECLARE)
		return (pro_strjoin(NULL, var));
	else if (type == APPEND)
	{
		while (var[i] && var[i] != '=')
			i++;
		if (!old_var_ptr || *(*old_var_ptr))
		{
			var[i - 1] = '\0';
			return (pro_strjoin(var, var + i));
		}
		else if (*var == '=' && ft_strchr(*old_var_ptr, '='))
			return (pro_strjoin(*old_var_ptr, var + 1));
		else if (*var == '=' && !ft_strchr(*old_var_ptr, '='))
			return (pro_strjoin(*old_var_ptr, var));
	}
	return (NULL);
}

static void	exporting(t_minishell *minishell, char *arg, char *var_name,
		t_export export_type)
{
	char		**existing_var;
	char		*var;

	existing_var = get_env_var(var_name, minishell->export_data);
	var = export_append(existing_var, arg, export_type);
	mem_move(m_info(0, 1, var, ENV_SCOPE));
	if (export_type == DECLARE && existing_var)
		return ;
	if (ft_strncmp(var, "_=", 2))
		minishell->export_data = add_or_replace_elem(minishell->export_data,
				var, var_name, FALSE);
	if (export_type != DECLARE)
		minishell->envp = add_or_replace_elem(minishell->envp,
				var, var_name, TRUE);
}

int	export(t_minishell *minishell, char **args)
{
	char		*var_name;
	char		*arg;
	t_export	export_type;
	int			i;

	i = 1;
	if (!(args[i]))
		return (print_export_data(minishell));
	while ((args[i]))
	{
		arg = args[i];
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
		i++;
	}
	return (0);
}
