/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 14:29:11 by bamrouch          #+#    #+#             */
/*   Updated: 2023/05/27 14:47:36 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *replace_args(char *arg, t_minishell *mini)
{
    arg = get_var(arg, mini);
    arg = unwrap_quotes(arg, mini);
    return arg;
}