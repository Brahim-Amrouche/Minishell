/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elasce <elasce@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:28:26 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/14 17:09:31 by elasce           ###   ########.fr       */
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

extern t_signal_var	g_sigvar;

//signals.c
void			handle_sigquit(int sig);
void			handle_sigint(int sig);

// main_utils.c
void			exit_on_empty_line(char *line);
t_minishell		*get_minishell(t_minishell *mini);
void			reset_minishell(t_minishell *minishell);
t_boolean		is_spaces_line(char *line);

// exit_minishel.c
int				return_msg(int stat, char *msg, ...);
void			print_msg(int fd, char *msg, ...);
void			exit_minishell(int err_n, char *message, t_boolean purge);
char            *get_var2(char *arg, t_minishell *mini, size_t *i, size_t j);

#endif