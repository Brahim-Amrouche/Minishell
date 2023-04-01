/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 19:57:04 by maboulkh          #+#    #+#             */
/*   Updated: 2023/04/01 22:22:46 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_prompt(t_boolean is_root)
{
	char	*prompt;
	char	*name;
	char	*identifier;

	name = "minishell";
	if (is_root)
		identifier = "# ";
	else
		identifier = "$ ";
	prompt = ft_strjoin(name, identifier);
	if (!prompt)
		exit(1);
	return (prompt);
}
