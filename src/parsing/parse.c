/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 19:50:21 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/15 15:24:12 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static t_boolean	handle_redirections_parsing(t_list *tokens_list, t_minishell *mini)
{
	if (((char *)tokens_list->content)[0] == input_redirect 
		|| ((char *) tokens_list->content)[0] == output_redirect)
	{
		if(!mini->parser_helper.cmd_holder)
		{
			mini->parser_helper.cmd_holder = ft_malloc(sizeof(t_exec_node), m_info(NULL, 1, NULL, 0));
			if(!mini->parser_helper.cmd_holder)
				exit_minishell(ENOMEM, "couldn't malloc cmd_holder in redirections", TRUE);
			ft_bzero(mini->parser_helper.cmd_holder, sizeof(t_exec_node));
		}
		redirection_parser(tokens_list, mini->parser_helper.cmd_holder);
		return TRUE;
	}
	return FALSE;
}

static t_boolean	handle_new_cmd_parsing(t_list *tokens_list, t_minishell *mini)
{
	if (mini->parser_helper.next_is_bin)
	{
		if (!mini->parser_helper.cmd_holder)
		{
			mini->parser_helper.cmd_holder = ft_malloc(sizeof(t_exec_node), m_info(NULL, 1, NULL, 0));
			if(!mini->parser_helper.cmd_holder)
				exit_minishell(ENOMEM, "couldn't malloc cmd_holder", TRUE);
			ft_bzero(mini->parser_helper.cmd_holder, sizeof(t_exec_node));
		}
		binary_parser(tokens_list, mini, mini->parser_helper.cmd_holder);
		mini->parser_helper.next_is_bin = FALSE;
		return TRUE;
	}
	return FALSE;
}


static t_boolean	handle_logical_ops_parsing(t_list *tokens_list, t_minishell *mini)
{
	if (or_token(tokens_list->content) || and_token(tokens_list->content))
	{
		mini->parser_helper.pipes_list = add_element_to_array(mini->parser_helper.pipes_list, mini->parser_helper.cmd_holder, sizeof(t_exec_node));
		mini->parsed_cmds.op_cmds = add_element_to_array(mini->parsed_cmds.op_cmds, &mini->parser_helper.pipes_list, sizeof(t_exec_node *));
		if (or_token(tokens_list->content))
			mini->parser_helper.cmd_operator = LOGICAL_OR;
		else
			mini->parser_helper.cmd_operator = LOGICAL_AND;
		mini->parsed_cmds.operations = add_element_to_array(mini->parsed_cmds.operations, &mini->parser_helper, sizeof(t_logical_operators));
		mini->parser_helper.pipes_list = NULL;
		mini->parser_helper.next_is_bin = TRUE;
		mini->parser_helper.cmd_holder = NULL;
		return TRUE;
	}
	return FALSE;
}

static t_boolean	handle_pipe_parsing(t_list *tokens_list, t_minishell *mini)
{
	if (*(char *)tokens_list->content == pipe_token)
	{
		mini->parser_helper.pipes_list = add_element_to_array(mini->parser_helper.pipes_list, mini->parser_helper.cmd_holder, sizeof(t_exec_node));
		mini->parser_helper.next_is_bin = TRUE;
		mini->parser_helper.cmd_holder = NULL;
		return TRUE;
	}
	return FALSE;
}

void    parse_tokens(t_minishell *mini)
{
    t_list		*tokens_list;

	ft_bzero(&mini->parser_helper, sizeof(t_parser_helpers));
	ft_bzero(&mini->parsed_cmds, sizeof(t_operations));
    tokens_list = mini->tokens;
 	mini->parser_helper.next_is_bin = TRUE;
	while (tokens_list && *(char *)tokens_list->content)
    {   
		quotes_parser(tokens_list, mini);
		if (handle_redirections_parsing(tokens_list, mini))
			;
		else if (handle_new_cmd_parsing(tokens_list, mini))
			;
		else if (handle_logical_ops_parsing(tokens_list, mini))
			;
		else if (handle_pipe_parsing(tokens_list, mini))
			;
		else
			mini->parser_helper.cmd_holder->cmd = add_element_to_array(mini->parser_helper.cmd_holder->cmd, &tokens_list->content, sizeof(char *));
		if (!tokens_list->next)
		{
			mini->parser_helper.pipes_list = add_element_to_array(mini->parser_helper.pipes_list, mini->parser_helper.cmd_holder, sizeof(t_exec_node));
			mini->parsed_cmds.op_cmds = add_element_to_array(mini->parsed_cmds.op_cmds, &mini->parser_helper.pipes_list , sizeof(t_exec_node *));
		}
		tokens_list = tokens_list->next;
    }
	t_exec_node **oper_cmds = mini->parsed_cmds.op_cmds;
	t_exec_node *pipe_list;
	char		**cmd;
	while(*oper_cmds)
	{
		printf("new operation \n");
		pipe_list = *oper_cmds;
		while(pipe_list->cmd) // this is a technical issue for now and to disscuss
		{
			printf("new pipe args\n");
			cmd = pipe_list->cmd;
			while (*cmd)
			{
				printf("args |%s|\n", *cmd);
				cmd++;
			}
			pipe_list++;
		}
		oper_cmds++;
	}
}
