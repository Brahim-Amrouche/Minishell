/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 13:40:43 by bamrouch          #+#    #+#             */
/*   Updated: 2023/06/22 16:12:03 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_boolean	ft_is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
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

	env_name = protected_substr(arg, (*i) + 1, j - (*i));
	if (!env_name)
		exit_minishell(ENOMEM, "could't malloc env_name", TRUE);
	if (match_str("?", env_name))
	{
		env_val = ft_itoa(mini->cmd_status);
		ft_malloc(1, m_info(env_val, 1, NULL, 0));
	}
	else
		env_val = find_env_var(mini->envp, env_name, FALSE);
	ft_free_node(1, env_name);
	new_arg = replace_value_in_arg(arg, *i, j + 1, env_val);
	*i += ft_strlen(env_val) - 1;
	ft_free_node(1, arg);
	ft_free_node(1, env_val);
	return (new_arg);
}

static t_boolean	is_dollar_breaker(char c)
{
	if (c == DOLLAR_SIGN || ft_is_space(c) || c == DOUBLE_QUOTE
		|| c == SINGLE_QUOTE)
		return (TRUE);
	return (FALSE);
}

char	*get_var(char *arg, t_minishell *mini, t_boolean skip)
{
	size_t		i;
	size_t		j;
	t_boolean	replace;

	i = 0;
	while (*(arg + i))
	{
		if (skip && *(arg + i) == SINGLE_QUOTE)
			skip_quotes(arg, &i);
		j = i + 1;
		replace = FALSE;
		while (*(arg + i) == DOLLAR_SIGN && *(arg + j) && (j != i + 1
				|| !is_dollar_breaker(*(arg + j))))
		{
			replace = TRUE;
			if (is_dollar_breaker(*(arg + j + 1)))
				break ;
			j++;
		}
		if (replace)
			arg = replace_env_var(arg, mini, &i, j);
		i++;
	}
	return (arg);
}
