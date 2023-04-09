/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_execution.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:57:26 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/09 07:33:15 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_EXECUTION_H
# define MINI_EXECUTION_H
# include "minishell.h"

#define ENV_SCOPE 69

char	**copy_envp(char **envp);
char	**add_elem_to_arr(char **arr, char *new_elem);

// # ifndef t_bool
// #  define t_bool

// typedef enum e_bool
// {
// 	FALSE = 0,
// 	TRUE = 1
// }				t_bool;
// # endif // t_bool

int exec_cmd(t_minishell *minishell, char *cmd);

char *get_env_var(char *name, char **env);

int change_dir(t_list *token);
int echo(t_minishell *minishell, t_list *token);
int env(t_minishell *minishell);
int get_dir(void);

#endif // MINI_EXECUTIONG_H