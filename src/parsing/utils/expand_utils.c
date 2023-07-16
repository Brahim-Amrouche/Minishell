/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 02:37:25 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/16 07:52:36 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_quotes(char **argv, size_t *i, t_minishell *mini)
{
	size_t	k;
	char	*inside_quote;
	size_t	len;

	k = (*i);
	skip_quotes(*argv, &k);
	if (k - (*i) > 1)
		inside_quote = protected_substr(*argv, (*i) + 1, k - (*i) - 1);
	else
		inside_quote = pro_str_dup("");
	if ((*argv)[*i] == DOUBLE_QUOTE)
		inside_quote = get_var(inside_quote, mini, FALSE);
	len = ft_strlen(inside_quote);
	*argv = replace_value_in_arg(*argv, (*i), k + 1, inside_quote);
	ft_memset(inside_quote, '-', len);
	mini->n_parser_helper.map = replace_value_in_arg(mini->n_parser_helper.map,
			(*i), k + 1, inside_quote);
	(*i) += ft_strlen(inside_quote);
	(mini->n_parser_helper.flager)++;
}

static char	*trim_arg(char *str, t_boolean starting_space)
{
	char	*res;
	char	**split;
	int		i;

	res = pro_str_dup("");
	if (str && ft_is_space(str[0]) && (starting_space))
		res = pro_strjoin(res, " ");
	split = ft_split_multi_sep(str, ft_is_space);
	if (!split || !(*split))
		return (res);
	i = 0;
	if (split[i])
		res = pro_strjoin(res, split[i++]);
	while (split[i])
	{
		res = pro_strjoin(res, " ");
		res = pro_strjoin(res, split[i]);
		i++;
	}
	if (str && ft_is_space(str[ft_strlen(str) - 1]) && ft_strlen(str) != 1)
		res = pro_strjoin(res, " ");
	return (res);
}

static void	expend_outside_quote(char **argv, t_minishell *mini, size_t *from,
		size_t *i)
{
	char		*new_argv;
	size_t		len;
	size_t		n;

	if (!argv || !(*argv))
		return ;
	len = *i - *from;
	new_argv = protected_substr(*argv, *from, len);
	new_argv = get_var(new_argv, mini, FALSE);
	new_argv = trim_arg(new_argv, (*from) > 0);
	len = ft_strlen(new_argv);
	*argv = replace_value_in_arg(*argv, (*from), *i, new_argv);
	n = -1;
	while (new_argv[++n])
		if (new_argv[n] != '*')
			new_argv[n] = '-';
	mini->n_parser_helper.map = replace_value_in_arg(mini->n_parser_helper.map,
			(*from), *i, new_argv);
	*i = *from;
	(*from) += len;
}

static t_boolean	expand_args(char **argv, size_t *i, t_minishell *mini,
		char ***args)
{
	size_t	last_space;
	size_t	i_copy;

	i_copy = *i;
	while ((*argv)[*i] && (*argv)[*i] != DOUBLE_QUOTE
		&& (*argv)[*i] != SINGLE_QUOTE)
		(*i)++;
	if (*i == i_copy)
		return (FALSE);
	expend_outside_quote(argv, mini, &i_copy, i);
	while (*i < i_copy && (*argv)[*i])
	{
		if (ft_is_space((*argv)[*i]))
		{
			last_space = *i;
			add_argv(argv, args, i);
			if (ft_strnstr((*argv), " ", i_copy - last_space)
				|| ft_strnstr((*argv), "\t", i_copy - last_space))
				continue ;
			else
				break ;
		}
		(*i)++;
	}
	return (TRUE);
}

char	**expand_argv(char *argv, t_minishell *mini)
{
	size_t	i;
	char	**args;

	args = NULL;
	i = 0;
	mini->n_parser_helper.map = pro_str_dup(argv);
	mini->n_parser_helper.flager = 0;
	while (argv[i])
	{
		if (expand_args(&argv, &i, mini, &args))
			continue ;
		else if (argv[i] == DOUBLE_QUOTE || argv[i] == SINGLE_QUOTE)
		{
			expand_quotes(&argv, &i, mini);
			continue ;
		}
		i++;
	}
	if (*argv || (!*argv && mini->n_parser_helper.flager))
		add_argv(&argv, &args, &i);
	return (args);
}
