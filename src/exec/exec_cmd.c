/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:57:04 by maboulkh          #+#    #+#             */
/*   Updated: 2023/05/16 19:51:43 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **get_env_var(char *name, char **env)
{
	int	len;

	if (!name || !env || !*env)
		return (NULL);
	len = ft_strlen(name);
	while (*env)
	{
		// if (!ft_strncmp(*env, name, len) && (*env)[len] == '=')
		if (!ft_strncmp(*env, name, len) && ((*env)[len] == '=' || !(*env)[len]))
			return (env);
		env++;
	}
	return (NULL);
}


int	exec_cmd(t_minishell *minishell, char *cmd)
{
	// t_list *token;
	// // t_list *temp;

	// token = minishell->tokens;


	// t_exec_node **oper_cmds = minishell->parsed_cmds.op_cmds;
	// t_exec_node *pipe_list;
	// char		**cmd1;
	// while(*oper_cmds)
	// {
	// 	printf("new operation \n");
	// 	pipe_list = *oper_cmds;
	// 	while(pipe_list->cmd) // this is a technical issue for now and to disscuss
	// 	{
	// 		printf("new pipe args\n");
	// 		cmd1 = pipe_list->cmd;
	// 		while (*cmd1)
	// 		{
	// 			printf("args |%s|\n", *cmd1);
	// 			cmd1++;
	// 		}
	// 		pipe_list++;
	// 	}
	// 	oper_cmds++;
	// }
	// // temp = token;
	// // while (temp)
	// // {
    // //     printf("tokens ==> |%s|\n", (char *)temp->content);
    // //     temp = temp->next;
    // // }
	// if (!cmd)
	// 	exit_minishell(-1, NULL, TRUE);
	// else if (!cmd)
	// 	minishell->cmd_status = 0;
	// else if (!ft_strncmp(CD, cmd, ft_strlen(CD)))
	// 	minishell->cmd_status = change_dir(minishell, token);
	// else if (!ft_strncmp(PWD, cmd, ft_strlen(PWD)))
	// 	minishell->cmd_status = get_dir();
	// else if (!ft_strncmp(ENV, cmd, ft_strlen(ENV)))
	// 	minishell->cmd_status = env(minishell);
	// else if (!ft_strncmp(ECHO, cmd, ft_strlen(ECHO)))
	// 	minishell->cmd_status = echo(minishell, token);
	// else if (!ft_strncmp(EXPORT, cmd, ft_strlen(EXPORT)))
	// 	minishell->cmd_status = export(minishell, token->next);
	// else if (!ft_strncmp(UNSET, cmd, ft_strlen(UNSET)))
	// 	minishell->cmd_status = unset(minishell, token->next);
	// else if (!ft_strncmp(BASH_EXIT, cmd, ft_strlen(BASH_EXIT)))
	// 	exit_minishell(-1, NULL, TRUE);
	// return (minishell->cmd_status);
}
