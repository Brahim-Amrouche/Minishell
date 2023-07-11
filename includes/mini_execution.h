/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_execution.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:57:26 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/11 22:03:05 by maboulkh         ###   ########.fr       */
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
# define STOP_WITH_SIGQUIT 131
# define ERR_NO_F 126
# define ERR_NO_P 127
# define ERR_DEF 1
# define ERR_NO_RED_P 1
# define ERR_EXIT_OVER 255
# define DEFAULT_FILE_PERMISSIONS 0666

typedef struct s_signal_var
{
	t_boolean	readline_stop;
	t_boolean	exec_stop;
	t_boolean	sig_quit;
	t_boolean	in_child;
}				t_signal_var;

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

// cd
int				change_dir(t_minishell *minishell, char **args);
// echo
int				echo(char **args);
// env
int				env(t_minishell *minishell);
// pwd
int				get_dir(t_minishell *minishell);
// export
int				export(t_minishell *minishell, char **args);
// envp
char			**export_envp(t_minishell *minishell, char **envp);
char			***fetch_export_data(void);
// unset
int				unset(t_minishell *minishell, char **args);
// exit
int				exit_shell(char **args);
t_stat			try_convert_strtoll(const char *str, long long *number);
// env utils
char			**add_elem_to_arr(char **arr, char *new_elem);
char			**rm_arr_elem(char **arr, char **elem);
char			**get_env_var(char *name, char **env);
// utils
void			wait_all(pid_t last_proc, int *status);
t_boolean		match_str(const char *s1, const char *s2);
t_boolean		has_quotes(char *str);
t_boolean		is_directory(char *path);
void			check_parentises_syntax(t_exec_tree *tree);
// wildcard.c
char			**create_wildcard_arr(char *pattern);
char			**create_pattern_arr(char *pattern);
// redirection
int				read_here_docs(t_exec_tree *tree, t_minishell *minishell);
t_stat			handle_redir_fd(int fd, t_redir_info *redir, int *std);
t_stat			handle_heredoc(t_redir_info *redir, t_minishell *minishell,
					int *tree_std);
t_stat			handle_redirection(t_redir_info *redir, t_minishell *minishell,
					int *tree_std);
// execute tree
int				main_execution(t_minishell *minishell);
int				traverse_tree(t_exec_tree *tree, t_minishell *minishell);
void			exec_cmd(t_exec_tree *tree, t_minishell *minishell);
void			exec_and_or(t_exec_tree *tree, t_minishell *minishell);
void			exec_parentheses(t_exec_tree *tree, t_minishell *minishell);
void			exec_pipe(t_exec_tree *tree, t_minishell *minishell);
// export_utils.c
char			**add_or_replace_elem(char **arr, char *new_elem,
					char *var_name, t_boolean free);
int				mini_export(t_minishell *minishell, char *var);
int				print_export_data(t_minishell *minishell);

#endif // MINI_EXECUTIONG_H