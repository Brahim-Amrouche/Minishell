/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:02:11 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/10 08:19:57 by maboulkh         ###   ########.fr       */
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
	if (!ft_isalpha(*token++))
		return (FALSE);
	while (ft_isalnum(*token))
		token++;
	if (*token != '=')
		return (FALSE);

	return (TRUE);
}

int	export(t_minishell *minishell, t_list *token)
{
	char	*var;
	char	*var_name;
	size_t	var_len;
	char	**existing_var;

// identifier should must start with an alpha and doesnt contain special characters

	token = token->next;
	if (!token)
		return ;
	var_len = 0;
	while (((char*) token->content)[var_len] && ((char*) token->content)[var_len] != '=')
		var_len++;
	var_name = protected_substr(var, 0, var_len);
	if (check_export_token(token))
	{
		var = protected_substr(token->content, 0, -1);
		existing_var = get_env_var(var_name, minishell->envp);
		if (!*existing_var)
			minishell->envp = add_elem_to_arr(minishell->envp, var);
		else
			replace_elem(var, existing_var);
	}
	else if (ft_strchr(token, '='))
	{
		minishell->cmd_status = 1;
		print_msg(2, "minishell: export: `$=': not a valid identifier", );
	}
	ft_free_node(SUBSTR_SCOPE, var_name);
	if (token->next)
		export(minishell, token);
	//  waht about in case of error
	return (0);
}