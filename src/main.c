/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:22:56 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/09 07:02:09 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

# define PERMA_SCOPE 1000


int	main(int argc, char *argv[], char *envp[])
{
	char	*cmd;
	t_minishell minishell;

	(void) argc;
	(void) argv;
	ft_bzero(&minishell, sizeof(t_minishell));
	minishell.envp = copy_envp(envp);
	while (TRUE)
	{
		cmd = readline("minishell$ ");
		ft_malloc(1, m_info(cmd , 1, NULL, 0));
		// here comes the parsing
		main_parsing(cmd, &minishell);
		// here comes execution
		exec_cmd(&minishell, cmd);
		ft_free(1, FALSE);
	}
	return 0;
}
