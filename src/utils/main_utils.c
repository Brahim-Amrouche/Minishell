/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:00:18 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/11 13:34:51 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_boolean	is_spaces_line(char *line)
{
	if (!line)
		return (TRUE);
	while (*line == ' ' || *line == '\t')
		line++;
	if (!*line)
		return (TRUE);
	return (FALSE);
}

void	exit_on_empty_line(char *line)
{
	if (!line)
	{
		printf("exit\n");
		ft_free(0, TRUE);
		rl_clear_history();
		exit(0);
	}
}

t_minishell	*get_minishell(t_minishell *mini)
{
	static t_minishell	*minishell;

	if (mini)
		minishell = mini;
	return (minishell);
}

void	reset_minishell(t_minishell *minishell)
{
	int		status;
	char	**envp;

	envp = minishell->envp;
	if (minishell->parsing_err_code)
		status = minishell->parsing_err_code;
	else
		status = minishell->cmd_status;
	ft_bzero(minishell, sizeof(t_minishell));
	ft_bzero(&g_sigvar, sizeof(t_signal_var));
	minishell->envp = envp;
	minishell->cmd_status = status;
}
