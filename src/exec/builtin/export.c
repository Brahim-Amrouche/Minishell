/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:02:11 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/15 06:10:41 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char **replace_elem(char *new_elem, char **old_elem)
{
	ft_free_node(ENV_SCOPE, *old_elem);
	*old_elem = new_elem;
	return (old_elem);
}

static t_boolean check_export_token(char *token)
{
	if (!token)
		return (FALSE);
	if (*token != '_' && !ft_isalpha(*token++))
		return (FALSE);
	while (*token && (ft_isalnum(*token) || *token == '_'))
		token++;
	if (!*token || *token == '=')
		return (TRUE);
	return (FALSE);
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

int	export(t_minishell *minishell, t_list *token)
{
// need to check for append
	static char **export_data;
	char		*var;
	char		*var_name;
	size_t		var_len;
	char		**existing_var;

	if (!token)
	{
		print_export_data(export_data);
		return (0);
	}
	var_len = 0;
	while (((char*) token->content)[var_len] && ((char*) token->content)[var_len] != '=')
		var_len++;
	var_name = protected_substr(token->content, 0, var_len);
	if (check_export_token(token->content))
	{
		var = protected_substr(token->content, 0, -1);
		existing_var = get_env_var(var_name, minishell->envp);
		if (!existing_var || !*existing_var)
		{
			export_data = add_elem_to_arr(export_data, var);
			if (var[var_len] == '=')
				minishell->envp = add_elem_to_arr(minishell->envp, var);
		}
		else
		{
			replace_elem(var, get_env_var(var_name, export_data));
			if (var[var_len] == '=')
				replace_elem(var, existing_var);
		}
	}
	else if (ft_strchr(token->content, '='))
	{
		minishell->cmd_status = 1;
		print_msg(2, "minishell: export: `$=': not a valid identifier", var_name);
	}
	ft_free_node(SUBSTR_SCOPE, var_name);
	if (token->next)
		export(minishell, token->next);
	//  waht about in case of error
	return (0);
}
