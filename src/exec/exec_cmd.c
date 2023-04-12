/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:57:04 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/11 07:52:09 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **get_env_var(char *name, char **env)
{
	int	var_len;

	if (!name || *env)
		return (NULL);
	var_len = ft_strlen(name);
	while (*env)
	{
		printf("no sigfault in sight at |%s|\n", *env);
		// if (!ft_strncmp(*env, name, var_len) && (*env)[var_len] == '=')
		if (!ft_strncmp(*env, name, var_len))
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
		minishell->cmd_status = echo(minishell, token);
	else if (!ft_strncmp(EXPORT, cmd, ft_strlen(EXPORT)))
		minishell->cmd_status = export(minishell, token->next);
	else if (!ft_strncmp(BASH_EXIT, cmd, ft_strlen(BASH_EXIT)))
		exit_minishell(-1, NULL, TRUE);
	return (minishell->cmd_status);
}
