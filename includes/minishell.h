/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:28:26 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/04 21:46:17 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "ft_garbage_collector.h"
# include "mini_parsing.h"
# include "mini_execution.h"
# include "errno.h"

void		print_msg(int fd, char *msg, ...);
void		exit_minishell(int err_n, char *message, t_boolean purge);

#endif