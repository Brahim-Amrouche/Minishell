/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 22:28:12 by bamrouch          #+#    #+#             */
/*   Updated: 2023/04/09 20:34:30 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *find_env_var(char **envp, char *needle, t_boolean get_og)
{
    size_t	needle_len;
	char	*env_val;
    
    if(!needle)
        return NULL;
    needle_len = ft_strlen(needle);
    while (*envp)
    {
        if (!ft_strncmp(*envp, needle, needle_len) && *((*envp) + needle_len) == '=')
		{
            if (get_og)
                return (*envp + needle_len + 1);
			env_val = protected_substr(*envp, needle_len + 1, ft_strlen(*envp));
			if (!env_val)
            	exit_minishell(ENOMEM, "could'nt malloc env_val", TRUE);
            return env_val;
		}
        envp++;
    }
    return NULL;
}

static  char *replace_env_var(t_list *token_node, t_minishell *mini, size_t *i, size_t j)
{
    char	*env_name;
	char	*env_val;
    char    *token;

    token = token_node->content;
    env_name = protected_substr(token, (*i) + 1, j);
	if (!env_name)
		exit_minishell(ENOMEM, "could't malloc env_name", TRUE);
    env_val = find_env_var(mini->envp, env_name, FALSE);
	ft_free_node(1, env_name);
    token_node->content = replace_value_in_token(token, *i, (*i) + j + 1, env_val);
    *i += ft_strlen(env_val) - 1; 
    ft_free_node(1, token);
    ft_free_node(1, env_val); 
    return token_node->content;
}

void    get_var(t_list *token_node, t_minishell *mini)
{
    size_t	i;
    size_t  j;
    char    *token;

    i = 0;
    token = token_node->content;
    while (*(token + i))
    {
        if (*(token + i) == dollar_sign && *(token + i + 1) &&
            !ft_is_space(*(token + i + 1)))
        {
            j = 1;
            while (*(token + i + j))
            {
                if (*(token + i + j) == dollar_sign || ft_is_space(*(token + i + j)))
                {
                    j--;
                    break;
                }
                j++;
            }
            token = replace_env_var(token_node, mini, &i, j);
		}
        i++;
    }
}