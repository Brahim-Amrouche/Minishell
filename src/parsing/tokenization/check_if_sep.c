/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_if_sep.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 15:13:17 by bamrouch          #+#    #+#             */
/*   Updated: 2023/06/04 15:16:38 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_sep_helper(char *seprators, size_t *j)
{
	while (seprators[*j] && seprators[*j] != '\n')
		(*j)++;
	if (seprators[*j])
		(*j)++;
}

int	check_if_sep(char *s, char *seprators)
{
	size_t	j;
	size_t	i;

	j = 0;
	while (seprators[j])
	{
		i = 0;
		while (seprators[j] && seprators[j] != '\n' && s[i])
		{
			if (seprators[j] == s[i])
			{
				i++;
				j++;
			}
			else
				break ;
		}
		if (!seprators || seprators[j] == '\n')
			return (i);
		else
			check_sep_helper(seprators, &j);
	}
	return (-1);
}
