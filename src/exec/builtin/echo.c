/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:01:55 by maboulkh          #+#    #+#             */
/*   Updated: 2023/05/28 21:20:04 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(t_minishell *minishell, t_exec_info *node)
{
	char		*txt;
	t_boolean	new_line;
	int			i;

	i = 1;
	new_line = TRUE;
	if (*(node->content + i) && !ft_strncmp("-n", *(node->content + 1), 2))
	{
		new_line = FALSE;
		i++;
	}	
	while (*(node->content + i))
	{
		txt = *(node->content + i);
		if (!ft_strncmp("$?", txt, 2))
			printf("%d", minishell->cmd_status);
		else
			printf("%s", txt);
		i++;
		if (*(node->content + i))
			printf(" ");
	}
	if (new_line)
		printf("\n");
	return (0);
}
