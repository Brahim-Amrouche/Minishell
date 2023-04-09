/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:13:21 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/09 05:28:18 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"




static t_boolean   redirect_input(t_list *token_node, t_exec_node *exec_node)
{
    char *token;

    token = token_node->content;
    if(exec_node->input)
        
    return TRUE;
}




void    redirection_parser(t_list *token_node, t_exec_node *exec_node)
{
    char    *token;
    size_t  input_redir_size;

    token = token_node->content;
    
    if(input_heredoc(token))
        return;   
    else if (output_append(token))
        return;
    else if (redirect_input(token, exec_node))
        return;
}