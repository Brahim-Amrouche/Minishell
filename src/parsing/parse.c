/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 19:50:21 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/09 18:37:14 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"


void    parse_tokens(t_minishell *mini)
{
    t_list		*tokens_list;
    t_boolean	next_is_bin;
	t_list		*operations_list;
	t_list		*pipes_list;
	t_exec_node	*cmd_holder;
	
    tokens_list = mini->tokens;
	next_is_bin = TRUE;
    while (tokens_list && *(char *)tokens_list->content)
    {   
        get_var(tokens_list, mini);
		if (next_is_bin)
        {
			cmd_holder = ft_malloc(sizeof(t_exec_node), m_info(NULL, 1, NULL, 0));
			if(!cmd_holder)
				exit_minishell(ENOMEM, "couldn't malloc cmd_holder", TRUE);
			ft_bzero(cmd_holder, sizeof(t_exec_node));
			binary_parser(tokens_list, mini, cmd_holder);
			next_is_bin = FALSE;
        }
		else if (((char *)tokens_list->content)[0] == input_redirect 
			|| ((char *) tokens_list->content)[0] == output_redirect)
			redirection_parser(tokens_list, cmd_holder);
        tokens_list = tokens_list->next;
    }
}
