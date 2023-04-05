/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 22:28:12 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/05 08:38:28 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *find_env_var(char **envp, char *needle)
{
    size_t	needle_len;
	char	*env_val;
    
    if(!needle)
        return NULL;
    needle_len = ft_strlen(needle);
    while (*envp)
    {
        if (ft_strnstr(*envp, needle, needle_len))
		{
			env_val = protected_substr(*envp, needle_len + 1, ft_strlen(*envp));
			if (!env_val)
            	exit_minishell(ENOMEM, "could'nt malloc env_val", TRUE);
            return env_val;
		}
        envp++;
    }
    return NULL;
}


void    get_var(t_list *token_node, t_minishell *mini)
{
    size_t	i;
    size_t  j;
    char	*env_name;
	char	*env_val;
    char    *token;
    
    i = 0;
    token = token_node->content;
    while (*(token + i))
    {
        if (*(token + i) == dollar_sign && *(token + i + 1) &&
            !ft_is_space(*(token + i + 1)))
        {
            j = 1;
            while (*(token + i + j) && !ft_is_space(*token))
                j++;
			env_name = protected_substr(token, i + 1, j);
			if (!env_name)
				exit_minishell(ENOMEM, "could't malloc env_name", TRUE);
            env_val = find_env_var(mini->envp, env_name);
			ft_free_node(1, env_name);
            token_node->content = replace_value_in_token(token, i, j, env_val);
            i += ft_strlen(env_val) - 1; 
            ft_free_node(1, token);
            ft_free_node(1, env_val);
            token = token_node->content;
		}
        // printf("paths |%s|\n", token);
        i++;
    }
}