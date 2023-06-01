/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:01:55 by maboulkh          #+#    #+#             */
/*   Updated: 2023/06/01 19:25:53 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(t_minishell *minishell, char ** args)
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
