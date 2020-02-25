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

static t_node	*and_or_list(t_node *left_pipeline)
{
	t_node	*and_or;
	t_node	*right_pipeline;

	and_or = NULL;
	if (g_token && (g_token->type == AND_IF || g_token->type == OR_IF))
	{
		and_or = node_new(g_token);
		node_add_child(and_or, left_pipeline);
		while (!(g_token = get_next_token()))
			g_lexer.line_cont = 1;
		if (!(right_pipeline = pipeline()))
			return (NULL);//parse error already set by pipeline
		node_add_child(and_or, right_pipeline);
		and_or = and_or_list(and_or);
	}
	return (and_or ? and_or : left_pipeline);
}

t_node			*and_or(void)
{
	t_node	*left_pipeline;

	if (!(left_pipeline = pipeline()))
		return (NULL);
		//return (parse_error(NO_CMD_BEFORE_AND_OR, g_token->value->str, NULL));
	return (and_or_list(left_pipeline));
}
