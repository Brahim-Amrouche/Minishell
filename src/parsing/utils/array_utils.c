/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 04:30:17 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/09 22:05:54 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

size_t  array_size(void *array, size_t data_size)
{
    size_t   len;
    size_t   i;

   len = 0;
   if (!array)
		return len;
   while(TRUE)
   {
		i = 0;
		while (((char *)array)[(len * data_size) + i] == 0 && i < data_size)
			i++;
		if (i  == data_size)
			break;
		len++;
   }
    return len;
}

// should be a null pointed arr
void    *add_element_to_array(void **old_array, void *new_elem, size_t data_size)
{
    size_t  cpy_mem_size;
	size_t	array_len;
    void    *new_array;
    void    *temp;

	array_len = array_size(old_array, data_size);
    new_array = ft_malloc((array_len + 2) * data_size, m_info(NULL, 1, NULL, 0));
    if (!new_array)
        exit_minishell(ENOMEM, "couldn't add element to array", TRUE);
    cpy_mem_size = array_len * data_size;
    if (old_array)
        ft_memcpy(new_array, old_array, cpy_mem_size);
    temp = (char *) new_array + cpy_mem_size;
    ft_memcpy(temp, new_elem, data_size);
    temp = (char *) temp + data_size;
    ft_bzero(temp, data_size);
    printf("%s \n",*(char **)new_array);
    return new_array;
}

