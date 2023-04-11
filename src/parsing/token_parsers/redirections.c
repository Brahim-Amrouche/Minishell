/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:13:21 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/11 02:47:25 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

# define SIMPLE_SEP 1
# define DOUBLE_SEP 2


static  char *get_redirection_path(char *token, t_boolean multiple_char_token)
{
    size_t  file_path_len;
	char	*res;

    token++;
    if (multiple_char_token)
        token++;
    while (*token && ft_is_space(*token))
        token++;
    if(!*token)
        exit_minishell(-1, "This is an empty redirect", TRUE);
    file_path_len = 0;
    while (*token && !ft_is_space(*token))
    {
        file_path_len++;
        token++;
    }
	token-=file_path_len;
	res = protected_substr(token, 0, file_path_len);
	if (!res)
		exit_minishell(ENOMEM, "Couldn't malloc redirection path", TRUE);
	if (!file_path_len && !*res)
		exit_minishell(-1, "this is a smarter empty redirect", TRUE);
	return res;
}

static t_redirections *parse_into_t_redirection(char *token, size_t token_skip, t_redirections *redirections, int mode)
{
	t_redirections	*input_redi;

	input_redi = ft_malloc(sizeof(t_redirections),m_info(NULL, 1, NULL, 0));
    if (!input_redi)
        exit_minishell(-1, "couldn't mallloc input redirection", TRUE);
	input_redi->file = get_redirection_path(token, token_skip > 1);
    input_redi->mode = mode;
	redirections = add_element_to_array(redirections, input_redi, sizeof(t_redirections));
    return redirections;
}

void    redirection_parser(t_list *token_node, t_exec_node *exec_node)
{
    char    *token;
    size_t  i;
    char    token_specifier;

    token = token_node->content;
    i = 0;
    token_specifier = *token;
    while(token[i] == token_specifier)
        i++;
    if(i > 2)
        exit_minishell(-1, "see here you are acting smart ass >.<", TRUE);
    if(input_heredoc(token))
        exec_node->input = parse_into_t_redirection(token, 2, exec_node->input, O_TRUNC | O_CREAT | O_WRONLY | O_SYMLINK);
    else if (output_append(token))
        exec_node->output = parse_into_t_redirection(token, 2, exec_node->output, O_APPEND | O_CREAT | O_WRONLY | O_SYMLINK);
    else if (*token == input_redirect)
        exec_node->input = parse_into_t_redirection(token, 1, exec_node->input, O_RDONLY | O_SYMLINK);
    else if (*token == output_redirect)
        exec_node->output = parse_into_t_redirection(token, 1, exec_node->output, O_TRUNC | O_CREAT | O_WRONLY | O_SYMLINK);
}