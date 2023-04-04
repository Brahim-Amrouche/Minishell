/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:23:48 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/03 05:41:23 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_msg(int fd, char *msg, ...)
{
	va_list	ap;

	va_start(ap, msg);
	while (*msg)
	{
		if (*msg == '%')
			ft_putnbr_fd(va_arg(ap, int), fd);
		else if (*msg == '$')
			ft_putstr_fd(va_arg(ap, char *), fd);
		else
			ft_putchar_fd(*msg, fd);
		msg++;
	}
	ft_putchar_fd('\n', fd);
	va_end(ap);
}

void	exit_minishell(int err_n, char *message, t_boolean purge)
{
	if (purge)
		ft_free(0, purge);
	if (err_n >= 0)
		ft_putstr_fd(strerror(err_n), STDERR_FILENO);
	if (message)
		ft_putstr_fd(message, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	exit(1);
}
