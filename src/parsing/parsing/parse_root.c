/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_root.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 14:39:21 by bamrouch          #+#    #+#             */
/*   Updated: 2023/05/28 18:58:27 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    handle_args(char *token, t_minishell *mini)
{
    void    *temp;
    t_exec_tree *exec_node;
    
    exec_node = mini->exec_root;
    if (exec_node->type == LOGICAL_REDI)
    {
        exec_node = exec_tree_node(1, LOGICAL_NONE);
        if (!exec_node)
            exit_minishell(ENOMEM, "couldn't create a new node in args handling", TRUE);
        mini->exec_root->left =  exec_node;
    }
    if (exec_node->type == LOGICAL_NONE)
        exec_node->type = LOGICAL_EXEC;
    temp = exec_node->info.content;
    exec_node->info.content = add_element_to_array(temp, &token, sizeof(char *));
    if (!exec_node->info.content)
        exit_minishell(ENOMEM, "couldn't add a new arg to cmds array", TRUE);
    ft_free_node(1, temp);
}

t_exec_tree	*parsing_root(t_minishell *mini)
{
    t_list *tokens;
    char   *token_content;

    tokens = mini->tokens;
    mini->exec_root = exec_tree_node(1, LOGICAL_NONE);
    if (!mini->exec_root)
        exit_minishell(ENOMEM, "couldn't create a new node", TRUE);
    while (tokens)
    {
        token_content = tokens->content;
        if(*token_content == '(' || *token_content == ')')
        {
            handle_parenthese(tokens, mini);
            tokens = mini->tokens;
            continue;
        }
        else if (!ft_strncmp(token_content, "||", 2) || !ft_strncmp(token_content, "&&", 2) || *token_content == '|')
        {
            parse_logical_operators(tokens, mini, token_content);
            tokens = mini->tokens;
            continue;
        }
        else if (!ft_strncmp(token_content, "<<", 2) || !ft_strncmp(token_content, ">>", 2) || *token_content == '<' || *token_content == '>')
        {
            parse_redirections(tokens, mini);
            tokens = mini->tokens;
            continue;
        }
        else
            handle_args(token_content, mini);
        tokens = tokens->next;
    }
	return mini->exec_root;
}