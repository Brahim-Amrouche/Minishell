/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 13:16:54 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/11 13:32:35 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_parentises_syntax(t_exec_tree *tree)
{
	int	i;
	int	j;

	i = 0;
	while (tree->argv && tree->argv[i])
	{
		j = 0;
		while (tree->argv[i][j] == '-')
			j++;
		if (i > 0 && !(j % 2))
			return_msg(2, "#syntax error in expression (error token is \"$\")",
				tree->argv[i]);
		i++;
	}
	exit(1);
}
