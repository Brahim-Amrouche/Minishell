/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:02:16 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/11 22:51:25 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_dir(t_minishell *minishell)
{
	char	*cwd;
	char	**env_pwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		env_pwd = get_env_var("PWD", minishell->envp);
		if (!env_pwd || !*env_pwd)
			return (return_msg(1, "#can 't find current working dir"));
		printf("%s\n", *(env_pwd) + ft_strlen("PWD="));
	}
	else
		printf("%s\n", cwd);
	free(cwd);
	return (0);
}
