/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 15:00:18 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/12 08:27:48 by maboulkh         ###   ########.fr       */
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
	char	**export_data;

	envp = minishell->envp;
	export_data = minishell->export_data;
	if (minishell->parsing_err_code)
		status = minishell->parsing_err_code;
	else
		status = minishell->cmd_status;
	ft_bzero(minishell, sizeof(t_minishell));
	ft_bzero(&g_sigvar, sizeof(t_signal_var));
	minishell->export_data = export_data;
	minishell->envp = envp;
	minishell->cmd_status = status;
	g_sigvar.stat = &(minishell->cmd_status);
}
