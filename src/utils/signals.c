/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:14:59 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/04 15:15:36 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_signal_var	*get_sigvar(void)
{
	static t_signal_var	sigvar;

	return (&sigvar);
}

void	handle_sigquit(int sig)
{
	sig++;
	if ((*get_sigvar()).in_child)
	{
		if ((*get_sigvar()).readline_stop)
			(*get_sigvar()).exec_stop = TRUE;
		write(1, "Quit\n", 5); // here or stderr?
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
