/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:02:11 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/09 07:57:47 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export(t_minishell *minishell, t_list *token)
{
	char	*var;
	char	*var_name;
	size_t	var_len;
	char	*existing_var;

// identifier should must start with an alpha and doesnt contain special characters

	token = token->next;
	if (!token)
		return ;
	var = token->content;
	var_len = 0;
	while (var[var_len] && var[var_len] != '=')
		var_len++;
	var_name = protected_substr(var, 0, var_len);
	existing_var = get_env_var(var_name, minishell->envp);
	if (!existing_var)
		minishell->envp = add_elem_to_arr(minishell->envp, var);
	else
	{
		// ft_free();
		// replace var;
	}
	//  waht about in case of error
	return (0);
}