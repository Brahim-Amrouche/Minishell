/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 13:40:43 by bamrouch          #+#    #+#             */
/*   Updated: 2023/06/04 19:17:24 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_boolean	ft_is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')// is this right
		return (TRUE);
	return (FALSE);
}

char	*find_env_var(char **envp, char *needle, t_boolean get_og)
{
	size_t	needle_len;
	char	*env_val;

	if (!needle || !envp)
		return (NULL);
	needle_len = ft_strlen(needle);
	while (*envp)
	{
		if (!ft_strncmp(*envp, needle, needle_len) && *((*envp)
				+ needle_len) == '=')
		{
			if (get_og)
				return (*envp + needle_len + 1);
			env_val = protected_substr(*envp, needle_len + 1, ft_strlen(*envp));
			if (!env_val)
				exit_minishell(ENOMEM, "could'nt malloc env_val", TRUE);
			return (env_val);
		}
		envp++;
	}
	return (NULL);
}

static char	*replace_env_var(char *arg, t_minishell *mini, size_t *i, size_t j)
{
	char	*env_name;
	char	*env_val;
	char	*new_arg;

	env_name = protected_substr(arg, (*i) + 1, j);
	if (!env_name)
		exit_minishell(ENOMEM, "could't malloc env_name", TRUE);
	if (!ft_strncmp("?", env_name, -1))
	{
		env_val = ft_itoa(mini->cmd_status);
		ft_malloc(1, m_info(env_val, 1, NULL, 0));// check this broooo!
	}
	else
		env_val = find_env_var(mini->envp, env_name, FALSE);
	ft_free_node(1, env_name);
	new_arg = replace_value_in_arg(arg, *i, (*i) + j + 1, env_val);
	*i += ft_strlen(env_val) - 1;
	ft_free_node(1, arg);
	ft_free_node(1, env_val);
	return (new_arg);
}

void	skip_quotes(char *arg, size_t *i)
{
	char	quote_type;
	int		back_up;

	back_up = *i;
	if (*(arg + *i) == DOUBLE_QUOTE || *(arg + *i) == SINGLE_QUOTE)
	{
		quote_type = *(arg + *i);
		(*i)++;
		while (*(arg + *i) && *(arg + *i) != quote_type)
			(*i)++;
		if (*(arg + *i) == '\0')
			*i = back_up;
	}
}

char	*get_var(char *arg, t_minishell *mini)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (*(arg + i))
	{
		skip_quotes(arg, &i);
		j = i + 1;
		if (*(arg + i) == DOLLAR_SIGN && *(arg + j) && !ft_is_space(*(arg + j)))
		{
			while (*(arg + j))
			{
				if (*(arg + j) == DOLLAR_SIGN || ft_is_space(*(arg + j)))
				{
					j--;
					break ;
				}
				j++;
			}
			arg = replace_env_var(arg, mini, &i, j);
		}
		i++;
	}
	return (arg);
}
