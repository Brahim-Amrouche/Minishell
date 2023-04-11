/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:57:04 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/11 01:43:13 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **get_env_var(char *name, char **env)
{
	int	var_len;

	var_len = ft_strlen(name);
	while (env)
	{
		if (!ft_strncmp(*env, name, var_len) && name[var_len] == '=')
			return (env);
		env++;
	}
	return (NULL);
}


int	exec_cmd(t_minishell *minishell, char *cmd)
{
	t_list *token;
	t_list *temp;

	token = minishell->tokens;

	temp = token;
	while (temp)
	{
        printf("tokens ==> |%s|\n", (char *)temp->content);
        temp = temp->next;
    }
	if (!cmd)
		exit_minishell(-1, NULL, TRUE);
	else if (!cmd)
		minishell->cmd_status = 0;
	else if (!ft_strncmp(CD, cmd, ft_strlen(CD)))
		minishell->cmd_status = change_dir(token);
	else if (!ft_strncmp(PWD, cmd, ft_strlen(PWD)))
		minishell->cmd_status = get_dir();
	else if (!ft_strncmp(ENV, cmd, ft_strlen(ENV)))
		minishell->cmd_status = env(minishell);
	else if (!ft_strncmp(ECHO, cmd, ft_strlen(ECHO)))
		minishell->cmd_status = echo(minishell, cmd);
	else if (!ft_strncmp(BASH_EXIT, cmd, ft_strlen(BASH_EXIT)))
		exit_minishell(-1, NULL, TRUE);
	return (minishell->cmd_status);
}
