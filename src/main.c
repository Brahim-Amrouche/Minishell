/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:22:56 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/04 06:21:04 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	char	*cmd;
	t_minishell minishell;

	(void) argc;
	(void) argv;


	while (TRUE)
	{
		ft_bzero(&minishell, sizeof(t_minishell));
		minishell.envp = envp;
		cmd = readline("minishell$");
		ft_malloc(1, m_info(cmd , 1, NULL, 0));
		// here comes the parsing
		main_parsing(cmd, &minishell);
		// here comes execution
		exec_cmd(cmd);
		ft_free(1, FALSE);
	}
	return 0;
}
