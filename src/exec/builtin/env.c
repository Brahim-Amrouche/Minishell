/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:01:59 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/02 23:33:29 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(t_minishell *minishell)
{
	char	**envp;

	mini_export(minishell, "_=/usr/bin/env");
	envp = minishell->envp;
	if (!envp)
		return (0);
	while (*envp)
	{
		printf("%s\n", *envp);
		++envp;
	}
	return (0);
}
