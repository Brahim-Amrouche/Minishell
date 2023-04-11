/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:01:08 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/09 03:06:53 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_dir(t_list *token)
{
	char	*path;

	path = NULL;
	if (token->next)
		path = token->next->content;
	if (path && !*path)
	path = ft_strdup("/Users/maboulkh");
	else if (path && *path != '/')
	path = ft_strjoin("./", path);
	else
		path = ft_strdup(path);
	if (access(path, F_OK) != 0)
		print_msg(2, "minishell: cd: $: No such file or directory", path);
	else if (access(path, X_OK) != 0)
		print_msg(2, "minishell: cd: $/: Permission denied", path);
	if (access(path, X_OK) != 0)
	{
		free(path);
		return (1);
	}
	chdir(path);
	free(path);
	return (0);
}
