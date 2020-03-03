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
** and_or			: pipeline AND_IF linebreak and_or
**					| pipeline OR_IF linebreak and_or
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

static void	linebreak_get_input(int operator_type)
{
	linebreak();
	while (!g_parser.error && !g_parser.token)
	{
		g_lexer.line_cont = operator_type;
		g_parser.token = get_next_token();
	}
}

t_node		*and_or(t_node *left_pipeline)
{
	t_node	*right_pipeline;
	t_node	*and_or_node;

	and_or_node = NULL;
	if (g_parser.error
	|| (!left_pipeline && !(left_pipeline = pipeline())))
		return (NULL);
	else if (g_parser.token
	&& (g_parser.token->type == AND_IF || g_parser.token->type == OR_IF))
	{
		and_or_node = node_new(g_parser.token, g_parser.token->type);
		node_add_child(and_or_node, left_pipeline);
		g_parser.token = get_next_token();
		linebreak_get_input(node_token(and_or_node)->type);
		right_pipeline = pipeline();
		node_add_child(and_or_node, right_pipeline);
		and_or_node = and_or(and_or_node);
	}
	return (and_or_node ? and_or_node : left_pipeline);
}
