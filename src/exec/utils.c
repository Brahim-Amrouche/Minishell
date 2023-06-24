/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 14:22:17 by maboulkh          #+#    #+#             */
/*   Updated: 2023/06/22 18:33:56 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_boolean	match_str(const char *s1, const char *s2)
{
	size_t	i;

	if (!s1 && !s2)
		return (TRUE);
	if (!s1 || !s2)
		return (FALSE);
	i = 0;
	while ((*(s1 + i) || *(s2 + i)))
	{
		if (((unsigned char)*(s1 + i)) != ((unsigned char)*(s2 + i)))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

t_boolean	has_quotes(char *str)
{
	if (!str)
		return (FALSE);
	while (*str)
	{
		if (*str == DOUBLE_QUOTE || *str == SINGLE_QUOTE)
			return (TRUE);
		str++;
	}
	return (FALSE);
}

void	wait_all(pid_t last_proc, int *status)
{
	pid_t	id;
	int		stat;

	id = wait(&stat);
	if (status && id == last_proc)
		*status = stat / 256;
	if (id != -1)
		wait_all(last_proc, status);
}

char	**get_env_var(char *name, char **env)
{
	int	len;

	if (!name || !env || !*env)
		return (NULL);
	len = ft_strlen(name);
	while (*env)
	{
		if (!ft_strncmp(*env, name, len) && ((*env)[len] == '='
				|| !(*env)[len]))
			return (env);
		env++;
	}
	return (NULL);
}
