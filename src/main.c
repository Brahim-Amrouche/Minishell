/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:22:56 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/03 00:00:21 by maboulkh         ###   ########.fr       */
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

t_signal_var *get_sigvar(void)
{
	static t_signal_var sigvar;
	return (&sigvar);
}

void	handle_sigquit(int sig)
{
	sig++;
	if ((*get_sigvar()).in_child)
	{
		if ((*get_sigvar()).readline_stop)
			(*get_sigvar()).exec_stop = TRUE;
		write(1, "Quit\n", 5);// here or stderr?
	}
	rl_redisplay();
}

void	handle_sigint(int sig)
{
	sig++;
	write(1, "\n", 1);
	if ((*get_sigvar()).readline_stop)
		(*get_sigvar()).exec_stop = TRUE;
	else
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	reset_minishell(t_minishell *minishell, t_signal_var *sigvar)
{
	int		status;
	char	**envp;

	envp = minishell->envp;
	if (minishell->parsing_err_code)
		status = minishell->parsing_err_code;
	else
		status = minishell->cmd_status;
	ft_bzero(minishell, sizeof(t_minishell));
	ft_bzero(sigvar, sizeof(t_signal_var));
	minishell->envp = envp;
	minishell->cmd_status = status;
}

t_minishell *get_minishell(t_minishell *mini)
{
	static	t_minishell *minishell;

	if (mini)
		minishell = mini;
	return minishell;
}

int	main(int argc, char *argv[], char *envp[])
{
	char			*cmd;
	t_minishell		minishell;
	struct termios	term;

	(void) argc;
	(void) argv;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	signal(SIGINT, &handle_sigint);
	signal(SIGQUIT, &handle_sigquit);
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
			minishell.cmd_status = 0;
			continue;
		}
		add_history(cmd);
		// here comes the parsing
		main_parsing(cmd, &minishell);
		// here comes execution
		if (!minishell.parsing_err_code)
		main_execution(&minishell);
		ft_free(1, FALSE);
		if ((*get_sigvar()).exec_stop)
			minishell.cmd_status = STOP_WITH_SIGINT;
	}
	rl_clear_history();
	return 0;
}
