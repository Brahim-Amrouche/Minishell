/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 04:30:17 by bamrouch          #+#    #+#             */
/*   Updated: 2023/05/16 19:41:14 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	*skip_bytes(void *pointer, size_t bytes)
{
    size_t  i;

    i = 0;
    while (i < bytes)
    {
        pointer++;
        i++;
    }
    return pointer;
}

static size_t	array_size(void *array, size_t data_size)
{
	size_t	len;
	size_t	i;
    char    *temp;

	len = 0;
	if (!array)
		return (len);
    i = 0;
    temp = array;
    while (TRUE && i < data_size)
	{
		i = 0;
		while (i < data_size && *(temp + i) == 0)
			i++;
		if (i == data_size)
			continue; 
		len++;
	    temp = skip_bytes(temp, data_size);
	}
	return (len);
}



// should be an array finished with a NULL or NULLISH value
void	*add_element_to_array(void *old_array, void *new_elem, size_t data_size)
{
	size_t	cpy_mem_size;
	size_t	array_len;
	void	*new_array;
	void	*temp;

	array_len = array_size(old_array, data_size);
	new_array = ft_malloc((array_len + 2) * data_size, m_info(NULL, 1, NULL,
				0));
	if (!new_array)
		exit_minishell(ENOMEM, "couldn't add element to array", TRUE);
	cpy_mem_size = array_len * data_size;
	if (old_array)
		ft_memcpy(new_array, old_array, cpy_mem_size);
	temp = skip_bytes(new_array, cpy_mem_size);
	ft_memcpy(temp, new_elem, data_size);
	temp = skip_bytes(temp, data_size);
	ft_bzero(temp, data_size);
	return (new_array);
}
