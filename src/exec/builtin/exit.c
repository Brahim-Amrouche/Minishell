/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:02:07 by maboulkh          #+#    #+#             */
/*   Updated: 2023/05/28 21:20:04 by maboulkh         ###   ########.fr       */
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

int exit_shell(t_exec_info *node)
{
	long long	status;
	char		*arg;
	t_boolean	err;

	err = FALSE;
	printf("exit\n");
	arg = node->content[1];
	if (!arg)
	{
		ft_free(0, TRUE);
		exit(0);
	}
	if (try_convert_strtoll(arg, &status))
	{
		err = TRUE;
		status = 2;
	}
	arg = node->content[2];
	if (!err && arg)
	{
		print_msg(2, "exit: too many arguments");
		return (1);
	}
	if (err)
		print_msg(2, "exit: $: numeric argument required", node->content[1]);
	status = status % 256;
	ft_free(0, TRUE);
	exit(status);
	return (status);
}
