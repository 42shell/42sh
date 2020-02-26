/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:32:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			is_process(t_node *node)
{
	return ((node->flags & PROCESS));// ??
}

void		process_del(t_process **process)
{
	size_t	i;

	i = 0;
	while ((*process)->argv && (*process)->argv[i])
		token_del(&(*process)->argv[i++]);
	i = 0;
	while ((*process)->redirs && (*process)->redirs[i])
		free_ast_nodes((*process)->redirs[i++], false);
	free((*process)->argv);
	free((*process)->redirs);
}

t_process	*process_new()
{
	t_process	*process;

	process = (t_process *)ft_xmalloc(sizeof(t_process));
	return (process);
}
