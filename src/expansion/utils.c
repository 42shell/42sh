/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:35:38 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_redir		*redir_dup(t_redir *redir)
{
	t_redir	*dup;

	if (!redir)
		return (NULL);
	dup = (t_redir *)ft_xmalloc(sizeof(t_redir));
	dup->left_op = token_dup(redir->left_op);
	dup->operator = token_dup(redir->operator);
	dup->right_op = token_dup(redir->right_op);
	dup->heredoc = token_dup(redir->heredoc);
	return (dup);
}

t_redir		*redir_list_dup(t_redir *list)
{
	t_redir	*dup;
	t_redir	*tmp;
	t_redir	*last;

	dup = NULL;
	if (!list)
		return (NULL);
	while (list)
	{
		tmp = redir_dup(list);
		if (!dup)
		{
			dup = tmp;
			last = dup;
		}
		else
		{
			last->next = tmp;
			last = last->next;
		}
		list = list->next;
	}
	return (dup);
}
