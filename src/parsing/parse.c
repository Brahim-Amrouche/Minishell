/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 19:50:21 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/07 04:05:36 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"


void    parse_tokens(t_minishell *mini)
{
    t_list		*tokens_list;
    t_boolean	next_is_bin;
    
    tokens_list = mini->tokens;
	next_is_bin = TRUE;
    while (tokens_list && *(char *)tokens_list->content)
    {   
        get_var(tokens_list, mini);
        
		if (next_is_bin)
        {
			binary_parser(tokens_list, mini);
            next_is_bin = FALSE;
        }
        tokens_list = tokens_list->next;
    }
}