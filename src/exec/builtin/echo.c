/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:01:55 by maboulkh          #+#    #+#             */
/*   Updated: 2023/05/18 19:55:13 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(t_minishell *minishell, t_exec_node *node)
{
	char		*txt;
	t_boolean	new_line;
	int			i;

	i = 1;
	new_line = TRUE;
	if (*(node->cmd + i) && !ft_strncmp("-n", *(node->cmd + 1), 2))
	{
		new_line = FALSE;
		i++;
	}	
	while (*(node->cmd + i))
	{
		txt = *(node->cmd + i);
		if (!ft_strncmp("$?", txt, 2))
			printf("%d", minishell->cmd_status);
		else
			printf("%s", txt);
		i++;
		if (*(node->cmd + i))
			printf(" ");
	}
	if (new_line)
		printf("\n");
	return (0);
}
