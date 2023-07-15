/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 13:40:43 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/15 22:01:27 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	size_t	arg_len;

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
	arg_len = ft_strlen(env_val);
	if (arg_len > 0)
		*i += arg_len - 1;
	ft_free_node(1, arg);
	ft_free_node(1, env_val);
	return (new_arg);
}

static	void	quotes_logic(char *arg, size_t *i, t_boolean skip,
	t_boolean *inside_quote)
{
	if (skip && *(arg + *i) == DOUBLE_QUOTE)
		*inside_quote = !(*inside_quote);
	if (skip && !(*inside_quote) && *(arg + *i) == SINGLE_QUOTE)
		skip_quotes(arg, i);
}

// static char	*replace_env_var2(char *arg, t_minishell *mini, size_t *i, size_t j)
// {
// 	char	*env_name;
// 	char	*env_val;
// 	char	*new_arg;


// 	env_name = protected_substr(arg, (*i) + 1, j - (*i));
// 	if (!env_name)
// 		exit_minishell(ENOMEM, "could't malloc env_name", TRUE);
// 	if (match_str("?", env_name))
// 	{
// 		env_val = ft_itoa(mini->cmd_status);
// 		ft_malloc(1, m_info(env_val, 1, NULL, 0));
// 	}
// 	else
// 		env_val = find_env_var(mini->envp, env_name, FALSE);
// 	ft_free_node(1, env_name);
// 	new_arg = replace_value_in_arg(arg, *i, j + 1, env_val);
// 	*i += ft_strlen(env_val) - 1;
// 	ft_free_node(1, arg);
// 	ft_free_node(1, env_val);
// 	return (new_arg);
// }

// char	*get_var2(char *arg, t_minishell *mini)
// {
//     size_t k;
//     size_t i;

//     i = 0;
// 	while (arg[i])
// 	{
// 		k = i + 1;
// 		if (arg[i] == DOLLAR_SIGN
// 			&& !ft_isdigit(arg[k]) && arg[k] != '?'
// 			&& is_dollar_char(arg[k]) && k++)
// 		{
// 			while (arg[k]
//                 && (is_dollar_char(arg[k]) || ft_isdigit(arg[k])))
// 				k++;
// 			k--;
// 			arg = replace_env_var2(arg, mini, &i, k);
// 		}
// 		else if (arg[i] == DOLLAR_SIGN
// 			&& (ft_isdigit(arg[k]) || arg[k] == '?'))
// 			arg = replace_env_var2(arg, mini, &i, k);
// 		i++;
// 	}
// 	return (arg);
// }

char	*get_var(char *arg, t_minishell *mini, t_boolean skip)
{
	size_t		i;
	size_t		j;
	t_boolean	inside_quote;

	i = 0;
	inside_quote = FALSE;
	while (arg[i])
	{
		quotes_logic(arg, &i, skip, &inside_quote);
		j = i + 1;
		if (arg[i] == DOLLAR_SIGN
			&& !ft_isdigit(arg[j]) && arg[j] != '?'
			&& is_dollar_char(arg[j]) && j++)
		{
			while (arg[j] && (is_dollar_char(arg[j]) || ft_isdigit(arg[j])))
				j++;
			j--;
			arg = replace_env_var(arg, mini, &i, j);
		}
		else if (arg[i] == DOLLAR_SIGN
			&& (ft_isdigit(arg[j]) || arg[j] == '?'))
			arg = replace_env_var(arg, mini, &i, j);
		i++;
	}
	return (arg);
}
