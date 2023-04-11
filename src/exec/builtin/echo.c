/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:01:55 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/09 07:08:30 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(t_minishell *minishell, t_list *token)
{
	char		*txt;
	t_boolean	new_line;

	token = token->next;
	new_line = TRUE;
	if (token && token->content && !ft_strncmp("-n", (char *) token->content, 2))
	{
		new_line = FALSE;
		token = token->next;
	}
	while (token)
	{
		txt = token->content;
		if (txt && !ft_strncmp("$?", txt, 2))
			printf("%d", minishell->cmd_status);
		else if (txt)
			printf("%s", txt);
		token = token->next;
		if (token)
			printf(" ");
	}
	if (new_line)
		printf("\n");
	return (0);
}
