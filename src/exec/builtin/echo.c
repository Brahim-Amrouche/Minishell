/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:01:55 by maboulkh          #+#    #+#             */
/*   Updated: 2023/06/22 18:35:11 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(char **args)
{
	char		*txt;
	t_boolean	new_line;
	int			i;

	i = 1;
	new_line = TRUE;
	if (*(args + i) && !ft_strncmp("-n", *(args + 1), 2))
	{
		new_line = FALSE;
		i++;
	}
	while (*(args + i))
	{
		txt = *(args + i);
		printf("%s", txt);
		i++;
		if (*(args + i))
			printf(" ");
	}
	if (new_line)
		printf("\n");
	return (0);
}
