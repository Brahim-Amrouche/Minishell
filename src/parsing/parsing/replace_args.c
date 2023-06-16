/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 14:29:11 by bamrouch          #+#    #+#             */
/*   Updated: 2023/06/16 16:48:52 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*replace_arg(char *arg, t_minishell *mini)
// {
// 	arg = get_var(arg, mini);
// 	arg = unwrap_quotes(arg, mini);
// 	return (arg);
// }

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

// typedef struct s_wrapper
// {
// 	char		*argv;
// 	char		***args;
// 	size_t		*i;
// 	size_t		*j;
// }				t_wrapper;

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

char	**replace_args(char **args, t_minishell *mini)
{
	char	**new_args;
	char	**temp_args;

	new_args = NULL;
	while (*args)
	{
		*args = get_var(*args, mini, TRUE);
		temp_args = new_args;
		new_args = add_arr_to_array(temp_args, split_argv_if_space(*args),
				sizeof(char *));
		ft_free_node(1, temp_args);
		args++;
	}
	ft_free_node(1, args);
	return (new_args);
}
