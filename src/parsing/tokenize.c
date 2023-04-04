/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 01:43:13 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/04 06:12:19 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static t_boolean check_for_tokens(char *c)
{
    if (*c == single_quote || *c == double_quote)
    {
        forced_token(FALSE, (t_tokenizer){*c, FALSE, FALSE, 1});
        return TRUE;
    }
    else if (*c == dollar_sign)
    {
        forced_token(FALSE, (t_tokenizer){0, FALSE, FALSE, 1});
        return TRUE;
    }
    else if (*c == input_redirect || *c == output_redirect)
    {
        forced_token(FALSE, (t_tokenizer){0, TRUE, FALSE, 1});
        if (*(c + 1) && (*(c + 1) == input_redirect || *(c + 1) == output_redirect ))
            forced_token(FALSE, (t_tokenizer){0, TRUE, FALSE, 2});
        return TRUE;
    }
    else if (*c == pipe_token)
    {
        forced_token(FALSE, (t_tokenizer){0, FALSE, TRUE, 1});
        if (*(c + 1) && *(c + 1) == pipe_token)
            forced_token(FALSE, (t_tokenizer) {0, FALSE, TRUE, 2});
        return TRUE;
    }
    else if (*c == bash_and && *(c + 1) && *(c + 1) == bash_and)
    {
        forced_token(FALSE, (t_tokenizer) {0, FALSE, TRUE, 2});
        return TRUE;
    }
    else if (ft_is_space(*c))
    {
        forced_token(FALSE, (t_tokenizer){0, FALSE, TRUE, 0});
        return TRUE;
    }
    return FALSE;
    
}    

void    tokenize_input(char *input, t_minishell *mini)
{
    t_list	*tokens_list;
    t_list	*token_node;
    char	*token;

    (void) mini;
    tokens_list = NULL;
    token = ft_strtok(input, check_for_tokens);
    while (token)
    {
        token_node = pro_lstnew(token);
        ft_lstadd_back(&tokens_list, token_node);
        token = ft_strtok(NULL, check_for_tokens);
    }
    while (tokens_list)
    {
        printf("all tokens === |%s|\n",(char *)tokens_list->content);
        tokens_list = tokens_list->next;
    }
}