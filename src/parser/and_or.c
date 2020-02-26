/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_or.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:32:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** and_or			: pipeline AND_IF and_or
**					| pipeline OR_IF and_or
**					| pipeline
**
** returns and_or in this format :
** ex:           ls && cat || ls && cat:
**
**                            "&&"
**                            /  \
**                         "||"   cat
**                         /  \
**                      "&&"   ls
**                      /  \
**                    ls   cat
*/

t_node			*and_or(void)
{
	t_node	*pipeline_node;
	t_node	*and_or_node;
	t_node	*next;

	and_or_node = NULL;
	if (!(pipeline_node = pipeline()))
		return (NULL);
	else if (g_token && (g_token->type == AND_IF || g_token->type == OR_IF))
	{
		and_or_node = node_new(g_token, 0);
		node_add_child(and_or_node, pipeline_node);
		while (!(g_token = get_next_token()))
			g_lexer.line_cont = 1;
		if (!(next = and_or()))
			return (NULL);
		node_add_child(and_or_node, next);
	}
	return (and_or_node ? and_or_node : pipeline_node);
}
