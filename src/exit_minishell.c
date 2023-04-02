/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:23:48 by bamrouch          #+#    #+#             */
/*   Updated: 2023/03/27 02:55:53 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    exit_minishell(int err_n, char *message, t_boolean purge)
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