/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:02:11 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/16 20:26:17 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char **replace_elem(char **arr, char *new_elem, char *var_name)
{
	char **old_elem;

	old_elem = get_env_var(var_name, arr);
	if (!new_elem)
		return (arr);
	if (!old_elem)
		return (add_elem_to_arr(arr, new_elem));
	// ft_free_node(ENV_SCOPE, *old_elem);
	*old_elem = new_elem;
	return (arr);
}

static t_export check_export_token(char *token)
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

// char *str_join_f(char *format, ...)
// {
// 	va_list	ap;
// 	char	**strings;
// 	char	*res;
// 	int		i;
// 	size_t	size;

// 	if (!format)
// 		return (NULL);
// 	va_start(ap, format);
// 	strings = NULL;
// 	i = 0;
// 	while (format[i])
// 		if (format[i++] == '$')
// 			strings = add_element_to_array(strings, va_arg(ap, char *), sizeof(char *));
// 	size = ft_strlen(format) - i;
// 	i = 0;
// 	while (strings && strings[i])
// 		size += ft_strlen(strings[i++]);
// 	ft_malloc(size + 1, m_info(NULL, STRJOIN_SCOPE, NULL, 0));
// 	i = 0;
// 	while (*format)
// 	{
// 		if (*format == '$')
// 		{
// 			while (*(*strings))
// 			{
// 				res[i++] = 'f';
// 			}
// 		}
// 	}
// }

static void print_export_data(char **export_data)
{
	char	**env_var;
	int		i;
	int		fd;

	fd = 1;
	if (!export_data)
		return ;
	env_var = export_data;
	while (*env_var)
	{
		i = 0;
		ft_putstr_fd("declare -x  ", fd);
		while ((*env_var)[i] && (*env_var)[i] != '=')
			ft_putchar_fd((*env_var)[i++], fd);
		if ((*env_var)[i] == '=')
			printf("=\"%s\"", (*env_var) + i + 1);
		printf("\n");
		env_var++;
	}
	return ;
}

char *get_export_variable_name(char *var, t_export type)
{
	size_t		len;

	if (!var)
		return (NULL);
	if (type == ERROR)
		return (protected_substr(var, 0, -1));
	len = 0;
	while (var[len] && var[len] != '=' && var[len] != '+')
		len++;
	return (protected_substr(var, 0, len));
}

char *export_append(char **old_var_ptr , char *var, t_export type)
{
	char *old_var;

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

int	export(t_minishell *minishell, t_list *token)
{
	static char **export_data;
	char		*var;
	char		*var_name;
	t_export	export_type;
	char		**existing_var;
	// char		**existing_var_exp;

	if (!token)
	{
		print_export_data(export_data);
		return (0);
	}
	export_type = check_export_token(token->content);
	var_name = get_export_variable_name(token->content, export_type);
	existing_var = get_env_var(var_name, export_data);
	var = export_append(existing_var , token->content, export_type);
	if (export_type)
	{
		export_data = replace_elem(export_data, var, var_name);
		if (export_type != DECLARE)
			minishell->envp = replace_elem(minishell->envp, var, var_name);
	}
	else
	{
		minishell->cmd_status = 1;
		print_msg(2, "minishell: export: `$': not a valid identifier", var_name);
	}
	// else if (export_type == ASSIGN ||  export_type == APPEND)
	// {
	// 	printf("assign/append\n");
	// 	export_data = replace_elem(export_data, var, var_name);
	// 	minishell->envp = replace_elem(minishell->envp, var, var_name);
	// }
	// else if (export_type == APPEND)
	// {
	// 	printf("append\n");
	// 	export_data = replace_elem(var, existing_var_exp, export_data);
	// 	minishell->envp = replace_elem(var, existing_var, minishell->envp);
	// }

	ft_free_node(SUBSTR_SCOPE, var_name);
	if (token->next)
		export(minishell, token->next);
	//  waht about in case of error
	return (0);
}
