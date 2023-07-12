/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:14:59 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/12 09:01:45 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigquit(int sig)
{
	sig++;
	if (g_sigvar.in_child)
	{
		if (g_sigvar.readline_stop)
		{
			g_sigvar.sig_quit = TRUE;
			g_sigvar.exec_stop = TRUE;
		}
		write(2, "Quit\n", 5);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
	else
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

void	handle_sigint(int sig)
{
	sig++;
	write(1, "\n", 1);
	if (g_sigvar.readline_stop)
		g_sigvar.exec_stop = TRUE;
	else
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		*(g_sigvar.stat) = 1;
	}
}
