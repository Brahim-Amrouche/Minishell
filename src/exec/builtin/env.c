/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:01:59 by maboulkh          #+#    #+#             */
/*   Updated: 2023/06/22 18:35:19 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(t_minishell *minishell)
{
	char	**envp;

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
