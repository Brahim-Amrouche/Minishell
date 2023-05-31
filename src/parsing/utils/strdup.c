/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strdup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 01:09:17 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/17 01:25:25 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*pro_str_dup(char *str)
{
	char	*res;
	size_t	str_len;
	size_t	i;

	str_len = ft_strlen(str);
	res = ft_malloc((str_len + 1) * sizeof(char), m_info(NULL, 1, NULL, 0));
	if (!res)
		return (NULL);
	i = 0;
	while (*(str + i))
	{
		*(res + i) = *(str + i);
		i++;
	}
	*(res + i) = 0;
	return (res);
}
