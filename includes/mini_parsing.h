/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_parsing.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:33:56 by bamrouch          #+#    #+#             */
/*   Updated: 2023/05/16 19:52:59 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_PARSING_H
# define MINI_PARSING_H
# include "minishell.h"
# include <readline/history.h>
# include <readline/readline.h>

# define ECHO "echo"
# define CD "cd"
# define PWD "pwd"
# define EXPORT "export"
# define UNSET "unset"
# define ENV "env"
# define BASH_EXIT "exit"

# define STRJOIN_SCOPE 1
# define SUBSTR_SCOPE 1
# define STRDUP_SCOPE 1

typedef enum s_logical_operators
{
	LOGICAL_NONE,
	LOGICAL_PIPE,
	LOGICAL_AND,
	LOGICAL_OR,
	LOGICAL_EXEC,
}						t_logical_operators;

typedef struct s_redirections
{
	char				*content;
	t_boolean			is_read;
	t_boolean			is_write;
	t_boolean			is_heredoc;
	t_boolean			is_append;
	t_boolean			continue_redirs;
}						t_redirections;

typedef struct s_exec_node
{
	char				**cmd;
	t_redirections		*input;
	t_redirections		*output;
}						t_exec_node;

typedef struct s_exec_info
{
	t_exec_node			*exec_node;
}						t_exec_info;

typedef struct s_exec_tree
{
	t_logical_operators	type;
	struct s_exec_tree	*parent;
	struct s_exec_tree	*left;
	struct s_exec_tree	*right;
	t_exec_info			info;
}						t_exec_tree;

typedef struct s_new_parser_helper
{
	t_list				*post_logic_token;
	t_boolean			prev_is_parenthese;
}						t_new_parser_helper;

typedef struct s_minishell
{
	t_list				*tokens;
	t_exec_tree			*exec_root;
	t_new_parser_helper	n_parser_helper;
	char				**envp;
	int					cmd_status;
}						t_minishell;

// new_tokenization
char					*str_tokenize(char *str, char *seperators);
void					tokenize(char *input, t_minishell *mini);

// new_parsing
t_exec_tree				*parsing_root(t_minishell *mini);
void					handle_parenthese(t_list *token_node,
							t_minishell *mini);
void					make_parenthese_tokens(t_list *parenthese_node,
							t_minishell *new_mini);
void					parse_redirections(t_list *redir_node,
							t_minishell *mini);
void					parse_logical_operators(t_list *logical_node,
							t_minishell *mini, char *token_content);

//	main_parsing.c
void					main_parsing(char *cmd, t_minishell *mini);

// env_variables.c
void					get_var(t_list *token_node, t_minishell *mini);
char					*find_env_var(char **envp, char *needle,
							t_boolean get_og);

//	binaries.c
t_boolean				cmd_is_builtin(char *cmd);
void					binary_parser(t_list *token_node, t_minishell *mini,
							t_exec_node *exec_node);

// utils
// substr.c
char					*protected_substr(char const *s, unsigned int start,
							size_t len);

// split_util.c
char	**ft_split_multi_sep(char *s,
							t_boolean (*sep_checker)(char));
// strdup.c
char					*pro_str_dup(char *str);

// strjoin.c
char					*pro_strjoin(char const *s1, char const *s2);

// replace_value_in_token.c
char					*replace_value_in_token(char *token, size_t pre,
							size_t post, char *env_val);

// pro_lstnew.c
t_list					*pro_lstnew(void *content);

// str_is_a_path.c
t_boolean				str_is_a_path(char *str);

// array_utils.c
void					*add_element_to_array(void *old_array, void *new_elem,
							size_t data_size);

// tree_utils.c
t_exec_tree				*exec_tree_node(int params_num, ...);

#endif
