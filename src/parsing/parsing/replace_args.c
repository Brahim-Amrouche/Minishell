/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 14:29:11 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/12 18:59:04 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_boolean	ft_is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (TRUE);
	return (FALSE);
}

static char	**replace_argv(char *argv, size_t *i, size_t *j)
{
	char	*new_arg;
	char	**new_args;

	new_arg = protected_substr(argv, *j, *i);
	new_args = create_wildcard_arr(new_arg);
	*j = *i;
	while (ft_is_space(argv[*j]))
		(*j)++;
	*i = *j;
	return (new_args);
}

static void	add_argv(char *argv, char ***args, size_t *i, size_t *j)
{
	char	**temp;
	char	**new_argv;

	new_argv = replace_argv(argv, i, j);
	temp = *args;
	*args = add_arr_to_array(temp, new_argv, sizeof(char *));
	ft_free_node(1, temp);
}

static char	**split_argv_if_space(char *argv, t_minishell *mini)
{
	size_t	i;
	size_t	j;
	char	**args;
	size_t	**quote_indexes;

	i = 0;
	args = NULL;
	j = 0;
	quote_indexes = mini->n_parser_helper.quote_indexes;
	while (argv[i])
	{
		if (quote_indexes && *quote_indexes
			&& i == (*quote_indexes)[0])
			i = (*(quote_indexes++))[1];
		else if (ft_is_space(argv[i]))
			add_argv(argv, &args, &i, &j);
		else
			i++;
	}
	add_argv(argv, &args, &i, &j);
	return (args);
}

char	**replace_args(char **args, t_minishell *mini)
{
	char	**new_args;
	char	**temp_args;
	char	**splited_args;

	new_args = NULL;
	mini->n_parser_helper.remove_quotes = TRUE;
	while (*args)
	{
		*args = get_var(*args, mini, TRUE);
		temp_args = new_args;
		splited_args = split_argv_if_space(*args, mini);
		new_args = add_arr_to_array(temp_args, splited_args,
				sizeof(char *));
		ft_free_node(1, temp_args);
		args++;
	}
	mini->n_parser_helper.remove_quotes = FALSE;
	ft_free_node(1, args);
	return (new_args);
}
