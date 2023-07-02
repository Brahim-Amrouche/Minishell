/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:28:26 by bamrouch          #+#    #+#             */
/*   Updated: 2023/06/23 15:57:09 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "ft_garbage_collector.h"
# ifdef LINUX
#  include <readline/history.h>
#  include <readline/readline.h>
#  include <sys/types.h>
#  include <sys/wait.h>
# else
#  include "history.h"
#  include "readline.h"
# endif // LINUX
# include "errno.h"
# include "mini_parsing.h"
# include "mini_execution.h"

void		print_msg(int fd, char *msg, ...);
t_minishell *get_minishell(t_minishell *mini);
int			return_msg(int stat, char *msg, ...);
void		exit_minishell(int err_n, char *message, t_boolean purge);

#endif