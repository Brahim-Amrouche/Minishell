/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 14:29:11 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/12 14:19:39 by bamrouch         ###   ########.fr       */
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
	size_t	n;

	new_arg = protected_substr(argv, *j, *i);
	new_args = create_wildcard_arr(new_arg);
	n = 0;
	while (new_args[n])
	{
		new_args[n] = unwrap_quotes(new_args[n]);
		n++;
	}
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

static char	**split_argv_if_space(char *argv)
{
	size_t	i;
	size_t	j;
	char	**args;

	i = 0;
	args = NULL;
	j = 0;
	while (argv[i])
	{
		if (ft_is_space(argv[i]))
		{
			add_argv(argv, &args, &i, &j);
			continue ;
		}
		else if (argv[i])
			skip_quotes(argv, &i);
		i++;
	}
	add_argv(argv, &args, &i, &j);
	return (args);
}


// char	**unwrap_double_quotes(char *args, t_minishell *mini)
// {
// 	size_t		i;
// 	size_t		j;
// 	size_t		n;
// 	char		*quoted_arg;
// 	char		**splited_arg;
// 	char		**arr_table;
// 	t_boolean	split;

// 	arr_table = NULL;
// 	i = 0;
// 	j = 0;
// 	while (TRUE)
// 	{
// 		j = i;
// 		while (args[i] != DOUBLE_QUOTE || args[i] == SINGLE_QUOTE || !args[i])
// 			i++;
// 		if (i > j)
// 		{
// 			quoted_arg = get_var(protected_substr(args, i, j));
// 			splited_arg = ft_split_multi_sep(quoted_arg, ft_is_space);
// 			n = 0;
// 			while (splited_arg && splited_arg[n])
// 				n++;
// 			args = splited_arg[n - 1];
// 			splited_arg[n - 1] = NULL;
// 			arr_table = add_arr_to_array(arr_table, splited_arg)
			
// 			args = replace_value_in_arg(args, i, j, quoted_arg);
// 			i = j + ft_strlen(quoted_arg);
// 			j = i;
// 		}
// 		if (!args[i])
// 			break ;
// 		if (args[i] == DOUBLE_QUOTE)
// 		{
// 			skip_quotes(args, &i);
// 			quoted_arg = get_var(protected_substr(args, i + 1, j - i - 1));
// 			args = replace_value_in_arg(args, i, j, quoted_arg);
// 			i = j + ft_strlen(quoted_arg) - 1;
// 		}
// 		else if (args[i] == SINGLE_QUOTE)
// 		{
// 			skip_quotes(args, &i);
// 			quoted_arg = protected_substr(args, i + 1, j - i - 1);
// 			args = replace_value_in_arg(args, i, j, quoted_arg);
// 			i = j + ft_strlen(quoted_arg) - 1;
// 		}
// 	}
// 	return (args);
// }

// char	**remove_double_quotes(char	*args,char	**splited_args, t_minishell *mini)
// {
// 	size_t	i;
// 	size_t	j;
// 	char	*quoted_args;


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
		splited_args = split_argv_if_space(*args);
		new_args = add_arr_to_array(temp_args, splited_args,
				sizeof(char *));
		ft_free_node(1, temp_args);
		args++;
	}
	ft_free_node(1, args);
	return (new_args);
}
