/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:23:48 by bamrouch          #+#    #+#             */
/*   Updated: 2023/06/13 18:43:52 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	return_msg(int stat, char *msg, ...)
{
	va_list	ap;

	va_start(ap, msg);
	while (*msg)
	{
		if (*msg == '%')
			ft_putnbr_fd(va_arg(ap, int), 2);
		else if (*msg == '$')
			ft_putstr_fd(va_arg(ap, char *), 2);
		else
			ft_putchar_fd(*msg, 2);
		msg++;
	}
	ft_putchar_fd('\n', 2);
	va_end(ap);
	return (stat);
}

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
