/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 03:27:22 by maboulkh          #+#    #+#             */
/*   Updated: 2023/05/25 23:13:13 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

char **add_elem_to_arr(char **arr, char *new_elem)
{
	char	**new_arr;
	size_t	size;
	size_t	i;

	if (!new_elem)
		return (arr);
	size = 0;
	while (arr && arr[size])
		size++;
	new_arr = ft_malloc((size + 2) *  sizeof(char *), m_info(NULL, ENV_SCOPE, NULL, 0));
	i = 0;
	while (arr && arr[i])
	{
		new_arr[i] = arr[i];
		i++;
	}
	new_arr[i++] = new_elem;
	mem_move(m_info(NULL, SUBSTR_SCOPE, new_elem, ENV_SCOPE));
	new_arr[i] = NULL;
	ft_free_node(ENV_SCOPE, arr);
	return (new_arr);
}

static char **add_or_replace_elem(char **arr, char *new_elem, char *var_name)
{
	char **old_elem;

	old_elem = get_env_var(var_name, arr);
	if (!new_elem)
		return (arr);
	if (!old_elem)
		return (add_elem_to_arr(arr, new_elem));
	// ft_free_node(ENV_SCOPE, *old_elem);
	*old_elem = new_elem;
	return (arr);
}

char *make_shell_lvl(int shell_lvl)
{
	char *res;
	char shell_lvl_str[100];
	int i;

	ft_bzero(shell_lvl_str, sizeof(shell_lvl_str));
	ft_memcmp(shell_lvl_str, "SHLVL=", 7);
	printf("shell _lvl = %s\n", shell_lvl_str);
	i = 7;
	if (shell_lvl > 99)
		shell_lvl_str[i++] = (shell_lvl / 100) + '0';
	if (shell_lvl > 9)
		shell_lvl_str[i++] = ((shell_lvl % 100) / 10) + '0';
	if (shell_lvl > 0)
		shell_lvl_str[i++] = (shell_lvl % 10) + '0';
	shell_lvl_str[i] = '\0';
	res = ft_strdup(shell_lvl_str);
	return (res);
}

char **add_essentiel_env(t_minishell *minishell)
{
	char *var;
	int shell_lvl;
	char **shell_lvl_str;

	var = ft_strdup("_=/usr/bin/env");
	minishell->envp = add_or_replace_elem(minishell->envp, var, NULL);
	shell_lvl = 1;
	shell_lvl_str = get_env_var("SHLVL", minishell->envp);
	if (shell_lvl_str)
		shell_lvl = ft_atoi((*shell_lvl_str) + 6) + 1;
	if (shell_lvl == 1000)
		shell_lvl = 0;
	else if (shell_lvl > 1000)
	{
		shell_lvl = 1;
		print_msg(2, "bash: warning: shell level (1001) too high, resetting to 1");
	}
	var = make_shell_lvl(shell_lvl);
	minishell->envp = add_or_replace_elem(minishell->envp, var, "SHLVL");
	return (minishell->envp);
}

char	**export_envp(t_minishell *minishell, char **envp)
{
	char *cmd[3];
	t_exec_node node;

	node.cmd = (char **) &cmd;
	cmd[0] = "export";
	cmd[2] = NULL;
	if (!envp)
		return (NULL);
	while (*envp)
	{
		cmd[1] = *envp;
		if (strncmp("_=", *envp, 2))
			export(minishell, &node, 0);
		envp++;
	}
	minishell->envp = add_essentiel_env(minishell);
	return (minishell->envp);
}
