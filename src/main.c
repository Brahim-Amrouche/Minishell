/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:22:56 by bamrouch          #+#    #+#             */
/*   Updated: 2023/05/25 16:20:15 by maboulkh         ###   ########.fr       */
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

void exit_on_empty_line(char *line)
{
	if (!line)
	{
		printf("exit\n");
		ft_free(0, TRUE);
		exit(0);
	}
}

int	*id_fetcher(void)
{
	static int id[3];
	return (id);
}

void	handle_sigquit(int sig)
{
	sig++;
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigkill(int sig)
{
	sig++;
	write(1, "\n", 1);
	id_fetcher()[2] = 1;
	rl_on_new_line();
	rl_replace_line("", 0);
	// if (id_fetcher()[1] > 0)
	// 	kill(id_fetcher()[1], SIGKILL);
	// if (id_fetcher()[0] > 0)
	// 	kill(id_fetcher()[0], SIGKILL);
	// if (id_fetcher()[0] <= 0)
	// {
	rl_redisplay();
	// }
}

int	main(int argc, char *argv[], char *envp[])
{
	char		*cmd;
	int			status;
	t_minishell minishell;

	(void) argc;
	(void) argv;
	signal(SIGINT, &handle_sigkill);
	signal(SIGQUIT, &handle_sigquit);
	status = 0;
	ft_bzero(&minishell, sizeof(t_minishell));
	envp = export_envp(&minishell, envp);
	while (TRUE)
	{
		ft_bzero(&minishell, sizeof(t_minishell));
		minishell.envp = envp;
		minishell.cmd_status = status;
		cmd = readline("\033[0;32mminishell$ \033[0m");
		ft_malloc(1, m_info(cmd, 1, NULL, 0));
		id_fetcher()[2] = 0;
		exit_on_empty_line(cmd);
		if (is_spaces_line(cmd))
		{
			minishell.cmd_status = 0;
			continue;
		}
		add_history(cmd);
		// here comes the parsing
		main_parsing(cmd, &minishell);
		// here comes execution
		main_execution(&minishell);
		ft_free(1, FALSE);
		envp = minishell.envp;
		status = minishell.cmd_status;
	}
	rl_clear_history();
	return 0;
}
