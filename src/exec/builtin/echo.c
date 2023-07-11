/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:01:55 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/11 17:22:18 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_boolean check_param(char **args, int *i)
{
	t_boolean	new_line;
	int			j;

	new_line = TRUE;
	while (*(args + *i) && !ft_strncmp("-n", *(args + *i), 2))
	{
		j = 2;
		while (args[*i][j] && args[*i][j] == 'n')
			j++;
		if (args[*i][j])
			break;
		new_line = FALSE;
		(*i)++;
	}
	return (new_line);
}

int	echo(char **args)
{
	char		*txt;
	t_boolean	new_line;
	int			i;

	i = 1;
	new_line = check_param(args, &i);
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
