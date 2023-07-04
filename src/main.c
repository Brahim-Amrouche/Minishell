/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:22:56 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/04 15:37:16 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	signals_init(struct termios *term)
{
	tcgetattr(STDIN_FILENO, term);
	(*term).c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, term);
	signal(SIGINT, &handle_sigint);
	signal(SIGQUIT, &handle_sigquit);
}

static	void	shell_execution(t_minishell *minishell, char *cmd)
{
	add_history(cmd);
	main_parsing(cmd, minishell);
	if (!minishell.parsing_err_code)
		main_execution(minishell);
	ft_free(1, FALSE);
	if ((*get_sigvar()).exec_stop)
		minishell->cmd_status = STOP_WITH_SIGINT;
}

int	main(int argc, char *argv[], char *envp[])
{
	char			*cmd;
	t_minishell		minishell;
	struct termios	term;

	((void) argc, (void) argv);
	signals_init(&term);
	ft_bzero(&minishell, sizeof(t_minishell));
	envp = export_envp(&minishell, envp);
	get_minishell(&minishell);
	while (TRUE)
	{
		reset_minishell(&minishell, get_sigvar());
		cmd = readline("\033[0;32mminishell$ \033[0m");
		ft_malloc(1, m_info(cmd, 1, NULL, 0));
		(*get_sigvar()).readline_stop = TRUE;
		exit_on_empty_line(cmd);
		if (is_spaces_line(cmd))
		{
			minishell.cmd_status = 0; // mr new parser is this really needed ??;
			continue ;
		}
		shell_execution(&minishell, cmd);
	}
	rl_clear_history();// is this mandatory to replicate in other places too??
	return (0);
}
