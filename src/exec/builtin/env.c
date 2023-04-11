/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:01:59 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/09 07:00:36 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(t_minishell *minishell)
{
	char		**envp;

	printf("env is here\n");
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
