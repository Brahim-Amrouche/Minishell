/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bamrouch <bamrouch@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:02:43 by bamrouch          #+#    #+#             */
/*   Updated: 2023/06/22 16:11:33 by bamrouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_boolean	get_redir_type(char *redir_type, t_redirection_types *type)
{
	t_boolean	parsed;

	parsed = TRUE;
	if (!ft_strncmp(redir_type, "<<", 2))
		*type = HERE_DOC_REDI;
	else if (!ft_strncmp(redir_type, ">>", 2))
		*type = APPEND_REDI;
	else if (*redir_type == '<')
		*type = INPUT_REDI;
	else if (*redir_type == '>')
		*type = OUTPUT_REDI;
	else
		parsed = FALSE;
	return (parsed);
}

void	parse_redirections(t_list *redir_node, t_minishell *mini)
{
	t_redir_info	*redir_info;
	t_redir_info	**old_redirs;

	if (!mini->exec_root->type)
		mini->exec_root->type = LOGICAL_EXEC;
	redir_info = ft_malloc(sizeof(t_redir_info), m_info(NULL, 1, NULL, 0));
	if (!redir_info)
		exit_minishell(ENOMEM, "couldn't malloc new redirection", TRUE);
	if (!redir_node->next)
		exit_minishell(-1, "give me a proper redirection", TRUE);
	if (get_redir_type(redir_node->next->content, &redir_info->redir_type))
		exit_minishell(-1, "unproper redirection", TRUE);
	get_redir_type(redir_node->content, &redir_info->redir_type);
	redir_info->content = redir_node->next->content;
	old_redirs = mini->exec_root->redir;
	mini->exec_root->redir = add_element_to_array(old_redirs, &redir_info,
			sizeof(t_redir_info *));
	if (!mini->exec_root->redir)
		exit_minishell(ENOMEM, "couldn't malloc new redirections table", TRUE);
	ft_free_node(1, old_redirs);
	mini->tokens = redir_node->next->next;
}
