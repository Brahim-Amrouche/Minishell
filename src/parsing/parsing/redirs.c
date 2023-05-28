/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   n_redirs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:02:43 by bamrouch          #+#    #+#             */
/*   Updated: 2023/05/28 19:14:29 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    set_redir_type(char *redir_type, t_exec_tree *redir_node)
{
    if (!ft_strncmp(redir_type, "<<", 2))
        redir_node->info.redir_type = HERE_DOC_REDI;
    else if (!ft_strncmp(redir_type, ">>", 2))
        redir_node->info.redir_type = APPEND_REDI;
    if (*redir_type == '<')
        redir_node->info.redir_type = INPUT_REDI;
    if (*redir_type == '>')
        redir_node->info.redir_type = OUTPUT_REDI;
}

void    parse_redirections(t_list *redir_node, t_minishell *mini)
{
    t_exec_tree *new_redir;
    
    if (mini->exec_root->type)
    {
        new_redir = exec_tree_node(3, LOGICAL_REDI, NULL, mini->exec_root);
        mini->exec_root = new_redir;
    }
    else
        new_redir = mini->exec_root;
    if(!new_redir)
        exit_minishell(ENOMEM, "couldn'nt malloc a new redir tree node", TRUE);
    if (!redir_node->next)
        exit_minishell(-1, "give me a proper redirection", TRUE);
    new_redir->type = LOGICAL_REDI;
    set_redir_type(redir_node->content, new_redir);
    new_redir->info.content = add_element_to_array(new_redir->info.content, &redir_node->next->content, sizeof(char *));
    if (!new_redir->info.content)
        exit_minishell(ENOMEM, "couldn't malloc a new redir content array", TRUE);
    if (mini->n_parser_helper.parenthese_node)
        new_redir->info.low_prio_redir = TRUE;
    mini->tokens = redir_node->next->next;
}