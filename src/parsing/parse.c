/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 19:50:21 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/11 04:29:41 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"


void    parse_tokens(t_minishell *mini)
{
    t_list		*tokens_list;
    t_boolean	next_is_bin;
	t_exec_node	*pipes_list;
	t_exec_node	*cmd_holder;
	t_logical_operators	cmd_operator;

    tokens_list = mini->tokens;
	next_is_bin = TRUE;
	pipes_list = NULL;
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
		else if (or_token(tokens_list->content) || and_token(tokens_list->content))
		{
			mini->parsed_cmds->op_cmds = add_element_to_array(mini->parsed_cmds->op_cmds, &pipes_list, sizeof(t_exec_node *));
			if (or_token(tokens_list->content))
				cmd_operator = LOGICAL_OR;
			else
				cmd_operator = LOGICAL_AND;
			mini->parsed_cmds->operations = add_element_to_array(mini->parsed_cmds->operations, &cmd_operator, sizeof(t_logical_operators));
			pipes_list = NULL;
			next_is_bin = TRUE;
		}
		else if (*(char *)tokens_list->content == pipe_token)
		{
			pipes_list = add_element_to_array(pipes_list, cmd_holder, sizeof(t_exec_node));
			next_is_bin = TRUE;
		}
		else
			cmd_holder->cmd = add_element_to_array(cmd_holder->cmd, &tokens_list->content, sizeof(char *));
		tokens_list = tokens_list->next;
    }
	// printf("|%s| \n", cmd_holder->input->file);
	// while (*cmd_holder->cmd)
	// {
	// 	printf("cmd_holder |%s|\n", *(cmd_holder->cmd));
	// 	cmd_holder->cmd++;
	// }
}
