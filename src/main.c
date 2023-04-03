/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:22:56 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/03 05:30:47 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	char	*cmd;
	
	while (TRUE)
	{
		cmd = readline("minishell$");
		ft_malloc(1, m_info(cmd , 1, NULL, 0));
		// here comes the parsing

		// here comes execution
		exec_cmd(cmd);
		ft_free(1, FALSE);
	}
	return 0;
}
