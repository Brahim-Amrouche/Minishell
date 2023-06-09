/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_value_in_token.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 00:04:02 by bamrouch          #+#    #+#             */
/*   Updated: 2023/06/16 16:29:58 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char    *replace_value_in_token(char *token, size_t  pre, size_t post,
// char *env_val)
// {
// 	char	*res;
// 	size_t	len;
// 	size_t	i;

// 	len = ft_strlen(token) - (post - pre) + ft_strlen(env_val);
// 	res = ft_malloc((len + 1) * sizeof(char),
// 							m_info(NULL, 1, NULL, 0));
// 	i = 0;
// 	while (i < pre)
// 	{
// 		res[i] = token[i];
// 		i++;
// 	}
// 	while (env_val && *env_val)
// 		res[i++] = *(env_val++);
// 	while (i < len)
// 		res[i++] = token[++post];
// 	res[i] = '\0';
// 	return (res);
// }

char	*replace_value_in_arg(char *token, size_t pre, size_t post,
		char *env_val)
{
	char	*pre_token;
	char	*res;
	char	*post_token;
	char	*temp_res;
	size_t	token_len;

	token_len = ft_strlen(token);
	pre_token = protected_substr(token, 0, pre);
	if (!pre_token)
		exit_minishell(ENOMEM, " couldn't malloc pre_token", TRUE);
	res = pro_strjoin(pre_token, env_val);
	if (!res)
		exit_minishell(ENOMEM, " couldn't join pre_token to res", TRUE);
	ft_free_node(SUBSTR_SCOPE, pre_token);
	post_token = protected_substr(token, post, token_len);
	if (!post_token)
		exit_minishell(ENOMEM, " couldn't malloc post_token", TRUE);
	temp_res = res;
	res = pro_strjoin(res, post_token);
	if (!res)
		exit_minishell(ENOMEM, " couldn't join post_token to res", TRUE);
	ft_free_node(STRJOIN_SCOPE, temp_res);
	ft_free_node(SUBSTR_SCOPE, post_token);
	return (res);
}
