/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_parsing.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:33:56 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/05 00:28:50 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_PARSING_H
# define MINI_PARSING_H
# include <readline/readline.h>
# include <readline/history.h>

# define ECHO "echo"
# define CD  "cd"
# define PWD  "pwd"
# define EXPORT  "export"
# define UNSET  "unset"
# define ENV  "env"
# define BASH_EXIT  "exit"

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
	t_list	*tokens;
	char	**envp;
}	t_minishell;

void			main_parsing(char *cmd, t_minishell *mini);
void			tokenize_input(char *input, t_minishell *mini);
void			parse_tokens(t_minishell *mini);
char			*find_env_var(char **envp, char *needle);
void			get_var(t_list *token_node, t_minishell *mini);



// utils
char	        *ft_strtok(char *str, t_boolean (*checker)(char *));
char			*protected_substr(char const *s, unsigned int start, size_t len);
char			**ft_split_multi_sep(char *s, t_boolean (*sep_checker)(char));
char			*pro_strjoin(char const *s1, char const *s2);
char			*replace_value_in_token(char *token, size_t  pre, size_t post, char *env_val);
t_list			*pro_lstnew(void *content);
t_tokenizer     forced_token(t_boolean read, ...);
t_boolean		ft_is_space(char c);

#endif
