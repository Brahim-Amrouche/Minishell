/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 04:19:09 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/07 04:37:18 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

t_boolean	or_token(char *c)
{
    if (*c == bash_or && *(c + 1) && *(c + 1) == bash_or)
        return TRUE;
    return FALSE;
}

t_boolean	and_token(char *c)
{
	if(*c == bash_and && *(c + 1) && *(c + 1) == bash_and)
		return TRUE;
	return FALSE;
}

t_boolean	input_heredoc(char *c)
{
	if(*c == input_redirect && *(c + 1) && *(c + 1) == input_redirect)
		return TRUE;
	return FALSE;
}

t_boolean	output_append(char *c)
{
	if(*c == output_redirect && *(c + 1) && *(c + 1) == output_redirect)
		return TRUE;
	return FALSE;
}
