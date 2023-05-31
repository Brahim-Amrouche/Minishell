/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strjoin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 23:41:36 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/07 03:06:59 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*pro_strjoin(char const *s1, char const *s2)
{
	size_t	total_len;
	size_t	s1_len;
	char	*res;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		s1 = "";
	if (!s2)
		s2 = "";
	s1_len = ft_strlen(s1) + 1;
	total_len = s1_len + ft_strlen(s2);
	res = ft_malloc(total_len * sizeof(char), m_info(NULL, STRJOIN_SCOPE, NULL,
				0));
	if (!res)
		return (NULL);
	res[0] = 0;
	ft_strlcat(res, s1, s1_len);
	ft_strlcat(res, s2, total_len);
	return (res);
}
