/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:02:07 by maboulkh          #+#    #+#             */
/*   Updated: 2023/12/27 15:52:55 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_stat	try_convert_strtoll(const char *str, long long *number)
{
	int			i;
	int			sign;
	long long	back_up;

	*number = 0;
	sign = 1;
	i = 0;
	if (str[i] == '-' || str[i] == '+')
		if ((++i || TRUE) && str[i - 1] == '-')
			sign = -1;
	if (str[i] < '0' || str[i] > '9')
		return (FAIL);
	while (str[i] >= '0' && str[i] <= '9')
	{
		back_up = *number;
		*number = *number * 10 + ((long long)(str[i] - '0')) * sign;
		if (*number / 10 != back_up)
			return (FAIL);
		++i;
	}
	if (str[i])
		return (FAIL);
	return (SUCCESS);
}

int	exit_shell(char **args)
{
	long long	status;
	t_boolean	do_exit;

	status = 0;
	if (g_sigvar.in_child == FALSE)
		printf("exit\n");
	do_exit = TRUE;
	if (args[1] && try_convert_strtoll(args[1], &status))
	{
		print_msg(2, "exit: $: numeric argument required", args[1]);
		status = ERR_EXIT_OVER;
	}
	else if (args[1] && args[2])
		do_exit = FALSE;
	if (do_exit)
	{
		ft_free(0, TRUE);
		rl_clear_history();
		exit(status % 256);
	}
	else
		print_msg(2, "exit: too many arguments");
	return (1);
}
