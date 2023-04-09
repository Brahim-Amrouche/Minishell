/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:02:16 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/09 03:13:34 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_dir(void)
{
	char	cwd[1024];

	if (getcwd(cwd, 1024) == NULL)
	{
		exit_minishell(1, "cwd buffer is not enough", TRUE);
	}
	else
		printf("%s\n", cwd);
	return (0);
}
