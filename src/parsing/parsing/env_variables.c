/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 13:40:43 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/13 06:21:13 by bamrouch         ###   ########.fr       */
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

void static	quote_logic_helper(char **arg, t_minishell *mini,
				size_t *i, t_boolean is_single)
{
	size_t		*quote_indexes;
	size_t		**temp;

	mini->n_parser_helper.quote_indexes = NULL;
	quote_indexes = ft_malloc(2 * sizeof(size_t), m_info(NULL, 1, NULL, 0));
	if (!quote_indexes)
		exit_minishell(ENOMEM, "couldnt malloc quote index", TRUE);
	quote_indexes[0] = *i;
	quote_indexes[1] = *i;
	skip_quotes(*arg, &quote_indexes[1]);
	*arg = remove_quotes(*arg, i, quote_indexes[1], is_single);
	quote_indexes[1] = (*i) + 1;
	temp = mini->n_parser_helper.quote_indexes ;
	mini->n_parser_helper.quote_indexes = add_element_to_array(temp, &quote_indexes, sizeof(size_t *));
	ft_free_node(1, temp);
}

static	void	quotes_logic(char **arg, size_t *i, t_boolean skip,
	t_boolean *inside_quote)
{
	t_minishell	*mini;

	mini = get_minishell(NULL);
	if (mini->n_parser_helper.remove_quotes && *((*arg) + *i) == DOUBLE_QUOTE)
		quote_logic_helper(arg, mini, i, TRUE);
	else if (mini->n_parser_helper.remove_quotes
		&& *((*arg) + *i) == SINGLE_QUOTE)
		quote_logic_helper(arg, mini, i, FALSE);
	else if (skip && *((*arg) + *i) == DOUBLE_QUOTE)
		*inside_quote = !(*inside_quote);
	else if (skip && !(*inside_quote) && *((*arg) + *i) == SINGLE_QUOTE)
		skip_quotes(*arg, i);
}

char	*get_var(char *arg, t_minishell *mini, t_boolean skip)
{
	size_t		i;
	size_t		j;
	t_boolean	inside_quote;

	i = 0;
	inside_quote = FALSE;
	while (arg[i])
	{
		quotes_logic(&arg, &i, skip, &inside_quote);
		if (i >= ft_strlen(arg))
			break ;
		j = i + 1;
		if (check_if_var(arg, i, j) && j++)
		{
			while (arg[j] && (is_dollar_char(arg[j]) || ft_isdigit(arg[j])))
				j++;
			j--;
			arg = replace_env_var(arg, mini, &i, j);
		}
		else if (check_if_special_var(arg, i, j))
			arg = replace_env_var(arg, mini, &i, j);
		i++;
	}
	return (arg);
}
