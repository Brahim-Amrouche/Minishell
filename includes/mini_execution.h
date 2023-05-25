/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_execution.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:57:26 by maboulkh          #+#    #+#             */
/*   Updated: 2023/05/25 21:39:10 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_EXECUTION_H
# define MINI_EXECUTION_H
# include "minishell.h"
# include <signal.h>


# ifndef O_SYMLINK
# define O_SYMLINK 0
# endif // O_SYMLINK

#define ENV_SCOPE 69


#define STOP_WITH_SIGINT 130

typedef struct s_signal_var
{
	t_boolean	readline_stop;
	t_boolean	exec_stop;
}	t_signal_var;

t_signal_var	state;

typedef struct s_env_var
{
	char *key;
	char *value;
}	t_env;


typedef enum e_export_type { ERROR = 0,
							 DECLARE = 1,
							 ASSIGN = 2,
							 APPEND = 3,
} t_export;

char	**copy_envp(char **envp);
char	**export_envp(t_minishell *minishell, char **envp);
char	**add_elem_to_arr(char **arr, char *new_elem);
char	**rm_elem_from_arr(char **arr, char **elem);

// # ifndef t_bool
// #  define t_bool

// typedef enum e_bool
// {
// 	FALSE = 0,
// 	TRUE = 1
// }				t_bool;
// # endif // t_bool

int main_execution(t_minishell *minishell);

# define DEFAULT_FILE_PERMISSIONS 0666

char **get_env_var(char *name, char **env);

int change_dir(t_minishell *minishell, t_exec_node *node);
int echo(t_minishell *minishell, t_exec_node *node);
int env(t_minishell *minishell);
int get_dir(void);
char ***fetch_export_data(void);
int export(t_minishell *minishell, t_exec_node *node, int index);
int unset(t_minishell *minishell, t_exec_node *node, int index);
int exit_shell(t_exec_node *node);
int *id_fetcher(void);

#endif // MINI_EXECUTIONG_H