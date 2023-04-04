/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pro_lstnew.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 20:39:10 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/03 20:42:40 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*pro_lstnew(void *content)
{
	t_list	*res;

	res = ft_malloc(sizeof(t_list), m_info(NULL, 1, NULL, 0));
    if (!res)
        return NULL;
	res->content = content;
	res->next = NULL;
	return (res);
}