/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_parsing.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 15:33:56 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/03 05:35:52 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_PARSING_H
# define MINI_PARSING_H
# include <readline/readline.h>
# include <readline/history.h>

typedef enum bash_tokens
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

typedef struct s_minishell
{
	char	**tokens;
	char	**envp;
}	t_minishell;

char	*ft_strtok(char *str, t_boolean (*checker)(char c));
char	*protected_substr(char const *s, unsigned int start, size_t len);
char	**ft_split_multi_sep(char *s, t_boolean (*sep_checker)(char));

#endif
