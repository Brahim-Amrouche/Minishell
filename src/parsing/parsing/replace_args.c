/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 14:29:11 by bamrouch          #+#    #+#             */
/*   Updated: 2023/07/16 07:09:45 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_boolean	ft_is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (TRUE);
	return (FALSE);
}

static char	**replace_argv(char **argv, size_t *i)
{
	char		*new_arg;
	char		**new_args;
	t_minishell	*mini;

	mini = get_minishell(NULL);
	new_arg = protected_substr(*argv, 0, *i);
	new_args = create_wildcard_arr(new_arg, &(mini->n_parser_helper.map));
	if (!new_args)
		new_args = add_element_to_array(new_args, &new_arg, sizeof(char *));
	while (ft_is_space((*argv)[*i]))
		(*i)++;
	*argv += (*i);
	mini->n_parser_helper.map += (*i);
	*i = 0;
	return (new_args);
}

void	add_argv(char **argv, char ***args, size_t *i)
{
	char		**temp;
	char		**new_argv;
	t_minishell	*mini;

	mini = get_minishell(NULL);
	if (mini->n_parser_helper.flager)
		mini->n_parser_helper.flager = 0;
	new_argv = replace_argv(argv, i);
	temp = *args;
	*args = add_arr_to_array(temp, new_argv, sizeof(char *));
	ft_free_node(1, temp);
}

char	**replace_args(char **args, t_minishell *mini)
{
	char	**new_args;
	char	**temp_args;
	char	**splited_args;

	new_args = NULL;
	while (*args)
	{
		temp_args = new_args;
		splited_args = expand_argv(*args, mini);
		new_args = add_arr_to_array(temp_args, splited_args,
				sizeof(char *));
		ft_free_node(1, temp_args);
		args++;
	}
	ft_free_node(1, args);
	return (new_args);
}
