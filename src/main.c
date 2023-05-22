/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:22:56 by bamrouch          #+#    #+#             */
/*   Updated: 2023/05/22 00:17:44 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

# define PERMA_SCOPE 1000

t_boolean	is_spaces_line(char *line)
{
	if (!line)
		return (TRUE);
	while (*line == ' ' || *line == '\t')
		line++;
	if (!*line)
		return (TRUE);
	return (FALSE);
}

int	main(int argc, char *argv[], char *envp[])
{
	char		*cmd;
	int			status;
	t_minishell minishell;

	(void) argc;
	(void) argv;
	status = 0;
	ft_bzero(&minishell, sizeof(t_minishell));
	envp = export_envp(&minishell, envp);
	while (TRUE)
	{
		ft_bzero(&minishell, sizeof(t_minishell));
		minishell.envp = envp;
		minishell.cmd_status = status;
		cmd = readline("minishell$ ");
		ft_malloc(1, m_info(cmd , 1, NULL, 0));
		if (is_spaces_line(cmd))
		{
			minishell.cmd_status = 0;
			continue;
		}
		add_history(cmd);
		// here comes the parsing
		main_parsing(cmd, &minishell);
		// here comes execution
		exec_cmd(&minishell);
		ft_free(1, FALSE);
		envp = minishell.envp;
	}
	rl_clear_history();
	return 0;
}
