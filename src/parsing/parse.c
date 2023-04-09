/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 19:50:21 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/09 02:39:01 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

t_boolean cmd_is_builtin(char *cmd)
{
	char    *builtin_list[8];
	size_t  cmd_len;
	size_t  i;

	i = 0;
	builtin_list[0] = ECHO;
	builtin_list[1] = CD;
	builtin_list[2] = PWD;
	builtin_list[3] = EXPORT;
	builtin_list[4] = UNSET;
	builtin_list[5] = ENV;
	builtin_list[6] = BASH_EXIT;
	builtin_list[7] = NULL;
	cmd_len = ft_strlen(cmd);
	while (builtin_list[i])
	{
		if (!ft_strncmp(cmd, builtin_list[i], cmd_len))
			return (TRUE);
		i++;
	}
	return (FALSE);
}

void    parse_tokens(t_minishell *mini)
{
    t_list		*tokens_list;
    t_boolean	next_is_bin;
    
    tokens_list = mini->tokens;
	next_is_bin = TRUE;
    while (tokens_list && *(char *)tokens_list->content)
    {   
        get_var(tokens_list, mini);
        
		if (next_is_bin && !cmd_is_builtin(tokens_list))
        {
			binary_parser(tokens_list, mini);
            next_is_bin = FALSE;
        }
        tokens_list = tokens_list->next;
    }
}
