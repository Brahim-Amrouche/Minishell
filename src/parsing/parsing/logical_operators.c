/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_operators.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 19:06:57 by bamrouch          #+#    #+#             */
/*   Updated: 2023/05/28 17:01:15 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void    make_logical_tokens(t_list *post_logic_node, t_minishell *new_mini, t_boolean is_pipe)
{
    t_list *new_node;
    char   *content;
    t_list *new_tokens;

    if(!post_logic_node)
        exit_minishell(-1, "i mean could u give another logical command", TRUE);
    content = post_logic_node->content;
    if(*content == '(' || *content == ')')
        make_parenthese_tokens(post_logic_node, new_mini);
    else
    {
        new_tokens = NULL;
        while (post_logic_node)
        {
            content = post_logic_node->content;
            if (!ft_strncmp(content, "&&", 2) || !ft_strncmp(content, "||", 2) || (is_pipe && *content == '|'))
                break;
            new_node = pro_lstnew(post_logic_node->content);
            if (!new_node)
                exit_minishell(ENOMEM, "couldn't malloc a new logical token node", TRUE);
            ft_lstadd_back(&new_tokens, new_node);
            post_logic_node = post_logic_node->next;
        }
        new_mini->n_parser_helper.post_logic_token = post_logic_node;
        new_mini->tokens = new_tokens;
    }
}



void	parse_logical_operators(t_list *logical_node, t_minishell *mini, char *token_content)
{
    t_exec_tree *new_root;
    t_minishell new_mini;
    size_t      token_len;

    ft_bzero(&new_mini, sizeof(t_minishell));
    token_len = ft_strlen(token_content);
    if(mini->exec_root->type)
    {
        make_logical_tokens(logical_node->next, &new_mini, token_len < 2);
        mini->tokens = new_mini.n_parser_helper.post_logic_token;
        if (!ft_strncmp(token_content, "&&", 2))
            new_root = exec_tree_node(4, LOGICAL_AND, NULL, mini->exec_root, parsing_root(&new_mini));
        else if (!ft_strncmp(token_content, "||", 2))
            new_root = exec_tree_node(4, LOGICAL_OR, NULL, mini->exec_root, parsing_root(&new_mini));
        else
            new_root = exec_tree_node(4,LOGICAL_PIPE, NULL, mini->exec_root, parsing_root(&new_mini));
        if (!new_root)
            exit_minishell(ENOMEM, "couldn't malloc a new pipe", TRUE);
        mini->exec_root->parent = new_root;
        mini->exec_root = new_root;
    }
    else
        exit_minishell(-1, "these logical operators are not well formated", TRUE);
    mini->n_parser_helper.parenthese_node = NULL;
}