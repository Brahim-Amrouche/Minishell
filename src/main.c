/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:22:56 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/11 16:05:45 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_signal_var	g_sigvar;

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
	if (!is_spaces_line(cmd))
	{
		main_parsing(cmd, minishell);
		if (!minishell->parsing_err_code)
			main_execution(minishell);
	}
	ft_free(1, FALSE);
	if (g_sigvar.exec_stop && g_sigvar.sig_quit)
		minishell->cmd_status = STOP_WITH_SIGQUIT;
	else if (g_sigvar.exec_stop)
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
		reset_minishell(&minishell);
		cmd = readline("minishell$ ");
		ft_malloc(1, m_info(cmd, 1, NULL, 0));
		g_sigvar.readline_stop = TRUE;
		exit_on_empty_line(cmd);
		shell_execution(&minishell, cmd);
	}
	rl_clear_history();
	return (0);
}
