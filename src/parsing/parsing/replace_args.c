/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 14:29:11 by bamrouch          #+#    #+#             */
/*   Updated: 2023/06/04 19:16:47 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace_args(char *arg, t_minishell *mini)
{
	arg = get_var(arg, mini);
	arg = unwrap_quotes(arg, mini);
	return (arg);
}
