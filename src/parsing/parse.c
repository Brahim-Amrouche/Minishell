/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 19:50:21 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/05 15:38:06 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"


void    parse_tokens(t_minishell *mini)
{
    t_list		*tokens_list;
    t_boolean	is_bin;
    
    tokens_list = mini->tokens;
	is_bin = TRUE;
    while (tokens_list && *(char *)tokens_list->content)
    {   
        get_var(tokens_list, mini);
		if (is_bin)
			
        tokens_list = tokens_list->next;
    }
}