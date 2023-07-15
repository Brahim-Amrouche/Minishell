/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 14:29:11 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/15 21:24:43 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_boolean	ft_is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (TRUE);
	return (FALSE);
}

static char	**replace_argv2(char **argv, size_t *i, char **map)
{
	char	*new_arg;
	char	**new_args;

	new_arg = protected_substr(*argv, 0, *i);
	new_args = create_wildcard_arr(new_arg, map);
	if (!new_args)
		new_args = add_element_to_array(new_args, &new_arg, sizeof(char *));
	while (ft_is_space((*argv)[*i]))
		(*i)++;
	*argv += (*i);
	*map += (*i);
	*i = 0;
	return (new_args);
}

static void	add_argv2(char **argv, char ***args,
				size_t *i, char **map)
{
	char	**temp;
	char	**new_argv;


	new_argv = replace_argv2(argv, i, map);
	temp = *args;
	*args = add_arr_to_array(temp, new_argv, sizeof(char *));
	ft_free_node(1, temp);
}

static void	split_argv_quotes_logic(char **argv, size_t *i, t_minishell *mini, char **map)
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
		inside_quote = get_var2(inside_quote, mini);
	len = ft_strlen(inside_quote);
	*argv = replace_value_in_arg(*argv, (*i), k + 1, inside_quote);
	ft_memset(inside_quote, '-', len);
	*map = replace_value_in_arg(*map, (*i), k + 1, inside_quote);
	(*i) += ft_strlen(inside_quote);
}

static void	expend_outside_qoute(char **argv, t_minishell *mini,
            size_t *from, char **map)
{
    char    *new_argv;
	size_t	len;
	size_t	i;
	size_t	n;

    if (!argv || !(*argv))
	{
        return ;
	}
	i = (*from);
	while ((*argv)[i] && (*argv)[i] != DOUBLE_QUOTE && (*argv)[i] != SINGLE_QUOTE)
        i++;
	len = i - *from;
    new_argv = protected_substr(*argv, *from, len);
    new_argv = get_var2(new_argv, mini);
	len = ft_strlen(new_argv);
	*argv = replace_value_in_arg(*argv, (*from), i , new_argv);
	n = -1;
	while (new_argv[++n])
		if (new_argv[n] != '*')
			new_argv[n] = '-';
	*map = replace_value_in_arg(*map, (*from), i , new_argv);
	(*from) += len;
}

static char	**split_argv(char *argv, t_minishell *mini)
{
	size_t	i;
	size_t	i_copy;
	size_t	expend_start;
	size_t	last_space;
	t_boolean has_space;
	char	**args;
	char	*map;

	args = NULL;
	i = 0;
	map = pro_str_dup(argv);
	while (argv[i])
	{
		i_copy = i;
        while (argv[i] && argv[i] != DOUBLE_QUOTE && argv[i] != SINGLE_QUOTE)
            i++;
		if (i != i_copy)
		{
			has_space = FALSE;
			expend_start = i_copy;
			expend_outside_qoute(&argv, mini, &i_copy, &map);
			i = expend_start;
			while (i < i_copy && argv[i])
			{
				if (ft_is_space(argv[i]))
				{
					has_space = TRUE;
					last_space = i;
					add_argv2(&argv, &args, &i, &map);// incrementing argv and i = 0
					continue ;
				}
				i++;
			}
			if (has_space)
				i = last_space;
			continue ;
		}
		else if (argv[i] == DOUBLE_QUOTE || argv[i] == SINGLE_QUOTE)
		{
			split_argv_quotes_logic(&argv, &i, mini, &map);
			continue ;
		}
		i++;
	}
	add_argv2(&argv, &args, &i, &map);
	return (args);
}

char	**replace_args(char **args, t_minishell *mini)
{
	char	**new_args;
	char	**temp_args;
	char	**splited_args;

	new_args = NULL;
	while (*args)
	{
		*args = get_var(*args, mini, TRUE);
		temp_args = new_args;
		splited_args = split_argv(*args, mini);
		new_args = add_arr_to_array(temp_args, splited_args,
				sizeof(char *));
		ft_free_node(1, temp_args);
		args++;
	}
	ft_free_node(1, args);
	return (new_args);
}
