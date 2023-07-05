/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_execution.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:57:26 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/05 12:53:02 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_EXECUTION_H
# define MINI_EXECUTION_H
# include "minishell.h"
# include <dirent.h>
# include <signal.h>
# include <termios.h>
# include <dirent.h>

# ifndef O_SYMLINK
#  define O_SYMLINK 0
# endif // O_SYMLINK

# define ENV_SCOPE 69

# define STOP_WITH_SIGINT 130
# define ERR_NO_F 126
# define ERR_NO_P 127
# define ERR_NO_RED_P 1

typedef struct s_signal_var
{
	t_boolean	readline_stop;
	t_boolean	exec_stop;
	t_boolean	in_heredoc;
	t_boolean	in_child;
}				t_signal_var;

t_signal_var	*get_sigvar(void);

typedef struct s_env_var
{
	char		*key;
	char		*value;
}				t_env;

typedef enum e_export_type
{
	ERROR = 0,
	DECLARE = 1,
	ASSIGN = 2,
	APPEND = 3,
}				t_export;

typedef enum e_stat
{
	SUCCESS = 0,
	FAIL = 1,
}				t_stat;

char			**copy_envp(char **envp);
char			**export_envp(t_minishell *minishell, char **envp);
char			**add_elem_to_arr(char **arr, char *new_elem);
char			**rm_elem_from_arr(char **arr, char **elem);
// wildcard.c
char			**create_wildcard_arr(char *pattern);
char			**create_pattern_arr(char *pattern);

int				read_here_docs(t_exec_tree *tree, t_minishell *minishell);
t_stat			handle_redir_fd(int fd, t_redir_info *redir, int *std);
t_stat			handle_heredoc(t_redir_info *redir, t_minishell *minishell,
					int *tree_std);
t_stat			handle_redirection(t_redir_info *redir, t_minishell *minishell,
					int *tree_std);
void			wait_all(pid_t last_proc, int *status);
t_boolean		match_str(const char *s1, const char *s2);
t_boolean		has_quotes(char *str);
t_boolean		is_directory(char *path);
int				traverse_tree(t_exec_tree *tree, t_minishell *minishell);
void			exec_cmd(t_exec_tree *tree, t_minishell *minishell);
void			exec_and_or(t_exec_tree *tree, t_minishell *minishell);
void			exec_parentheses(t_exec_tree *tree, t_minishell *minishell);
void			exec_pipe(t_exec_tree *tree, t_minishell *minishell);


int				main_execution(t_minishell *minishell);

# define DEFAULT_FILE_PERMISSIONS 0666


char			**get_env_var(char *name, char **env);
t_stat			try_convert_strtoll(const char *str, long long *number);

int				change_dir(t_minishell *minishell, char **args);
int				echo(char **args);
int				env(t_minishell *minishell);
int				get_dir(t_minishell *minishell);
char			***fetch_export_data(void);
int				exit_shell(char **args);
int				unset(t_minishell *minishell, char **args);
int				export(t_minishell *minishell, char **args);
// export_utils.c
char			**add_or_replace_elem(char **arr, char *new_elem, char *var_name, t_boolean free);
int				mini_export(t_minishell *minishell, char *var);
int				print_export_data(t_minishell *minishell);

#endif // MINI_EXECUTIONG_H