/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_parsing.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:33:56 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/09 04:01:57 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_PARSING_H
# define MINI_PARSING_H
# include <readline/readline.h>
# include <readline/history.h>
#include "minishell.h"

# define ECHO "echo"
# define CD  "cd"
# define PWD  "pwd"
# define EXPORT  "export"
# define UNSET  "unset"
# define ENV  "env"
# define BASH_EXIT  "exit"

# define STRJOIN_SCOPE 1
# define SUBSTR_SCOPE 1

typedef	enum s_logical_operators
{
	LOGICAL_OR ,
	LOGICAL_AND,
}	t_logical_operators;

typedef struct s_redirections
{
	char	*file;
	int		mode;
} t_redirections;


typedef struct	s_exec_node
{
	char			**cmd;
	t_redirections	*input;
	t_redirections	*output;
} t_exec_node;

typedef struct	s_operations
{
	t_exec_node			**op_cmds;
	t_logical_operators *operations;
} t_operations;


typedef enum e_bash_tokens
{
	single_quote = '\'',
	double_quote = '"',
	dollar_sign = '$',
	input_redirect = '<',
	output_redirect = '>',
	pipe_token = '|',
	bash_or = '|',
	bash_and = '&',
	wildcard = '*'
}	t_bash_tokens;

typedef struct s_tokenizer
{
	char		break_token;
	t_boolean	allows_space;
    t_boolean   ignore;
    size_t      step;
}   t_tokenizer;

typedef struct s_minishell
{
	t_list			*tokens;
	char			**envp;
	t_operations	*parsed_cmds;	
	int		cmd_status;
}	t_minishell;



//	main_parsing.c
void			main_parsing(char *cmd, t_minishell *mini);
//	tokenize.c
void			tokenize_input(char *input, t_minishell *mini);

// parse.c
void			parse_tokens(t_minishell *mini);

// env_variables.c
void			get_var(t_list *token_node, t_minishell *mini);
char			*find_env_var(char **envp, char *needle);

//	binaries.c
void			binary_parser(t_list *token_node, t_minishell *mini);



// utils
// ft_strtok.c
char	        *ft_strtok(char *str, t_boolean (*checker)(char *));
t_tokenizer     forced_token(t_boolean read, ...);
t_boolean		ft_is_space(char c);

// substr.c
char			*protected_substr(char const *s, unsigned int start, size_t len);

// split_util.c
char			**ft_split_multi_sep(char *s, t_boolean (*sep_checker)(char));

// strjoin.c
char			*pro_strjoin(char const *s1, char const *s2);

// replace_value_in_token.c
char			*replace_value_in_token(char *token, size_t  pre, size_t post, char *env_val);

// pro_lstnew.c
t_list			*pro_lstnew(void *content);

// str_is_a_path.c
t_boolean		str_is_a_path(char *str);

// special_tokens.c
t_boolean		or_token(char *c);
t_boolean		and_token(char *c);
t_boolean		input_heredoc(char *c);
t_boolean		output_append(char *c);

#endif
