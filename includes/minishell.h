/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:28:26 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/27 17:28:52 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "ft_garbage_collector.h"
# ifdef LINUX
#  include <sys/types.h>
#  include <sys/wait.h>
#  include <readline/readline.h>
#  include <readline/history.h>
# else
#  include "readline.h"
#  include "history.h"
#endif // LINUX
# include "mini_parsing.h"
# include "mini_execution.h"
# include "errno.h"

void		print_msg(int fd, char *msg, ...);
void		exit_minishell(int err_n, char *message, t_boolean purge);

#endif