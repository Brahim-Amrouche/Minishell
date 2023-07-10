/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elasce <elasce@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:14:59 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/10 16:27:44 by elasce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigquit(int sig)
{
	sig++;
	if (sigvar.in_child)
	{
		if (sigvar.readline_stop)
        {
            sigvar.sig_quit = TRUE;
			sigvar.exec_stop = TRUE;
        }
		write(1, "Quit\n", 5); // here or stderr?
	}
	rl_redisplay();
}

void	handle_sigint(int sig)
{
	sig++;
	write(1, "\n", 1);
	if (sigvar.readline_stop)
		sigvar.exec_stop = TRUE;
	else
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
