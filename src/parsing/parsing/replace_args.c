/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 14:29:11 by bamrouch          #+#    #+#             */
/*   Updated: 2023/06/16 16:23:04 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*replace_arg(char *arg, t_minishell *mini)
// {
// 	arg = get_var(arg, mini);
// 	arg = unwrap_quotes(arg, mini);
// 	return (arg);
// }

static char	**replace_argv(char *argv, size_t *i, size_t *j, t_minishell *mini)
{
	char	*new_arg;
	char	**new_args;
	size_t	n;

	new_arg = protected_substr(argv, *j, *i);
	new_args = create_wildcard_arr(new_arg, mini);
	n = 0;
	while (new_args[n])
	{
		new_args[n] = unwrap_quotes(new_args[n], mini);
		n++;
	}
	*j = *i;
	while (ft_is_space(argv[*j]))
		(*j)++;
	*i = *j;
	return (new_args);
}

typedef struct s_wrapper
{
	char		*argv;
	char		***args;
	size_t		*i;
	size_t		*j;
	t_minishell	*mini;
}				t_wrapper;

static char	**add_argv(t_wrapper wrapper)
{
	char	**temp;
	char	**new_argv;

	new_argv = replace_argv(wrapper.argv, wrapper.i, wrapper.j, wrapper.mini);
	temp = *wrapper.args;
	*wrapper.args = add_arr_to_array(temp, new_argv, sizeof(char *));
	ft_free_node(1, temp);
}

static char	**split_argv_if_space(char *argv, t_minishell *mini)
{
	size_t	i;
	size_t	j;
	char	**args;
	char	**new_argv;

	i = 0;
	args = NULL;
	j = 0;
	while (argv[i])
	{
		if (ft_is_space(argv[i]))
		{
			add_argv((t_wrapper){argv, &args, &i, &j, mini});
			continue ;
		}
		else if (argv[i])
			skip_quotes(argv, &i);
		i++;
	}
	add_argv((t_wrapper){argv, &args, &i, &j, mini});
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
		new_args = add_arr_to_array(temp_args, split_argv_if_space(*args, mini),
				sizeof(char *));
		ft_free_node(1, temp_args);
		args++;
	}
	ft_free_node(1, args);
	return (new_args);
}
