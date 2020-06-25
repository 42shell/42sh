/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_or.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/10 00:08:46 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static t_command	*build_and_or_and_advance(t_command *left)
{
	t_command		*node;
	int				old_linebreak_type;

	node = command_new(CONNECTION);
	node->value.connection->connector = g_parser.token->type;
	node->value.connection->left = left;
	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_parser.token = get_next_token();
	old_linebreak_type = g_linebreak_type;
	g_linebreak_type = node->value.connection->connector;
	parse_linebreak();
	g_linebreak_type = old_linebreak_type;
	return (node);
}

/*
** and_or          :                         pipeline
**                 | and_or AND_IF linebreak pipeline
**                 | and_or OR_IF  linebreak pipeline
*/

t_command			*parse_and_or(void)
{
	t_command		*and_or;
	t_command		*node;

	if (!g_parser.token || !(and_or = parse_pipeline()))
		return (NULL);
	while (g_parser.token
	&& (g_parser.token->type == AND_IF || g_parser.token->type == OR_IF))
	{
		node = build_and_or_and_advance(and_or);
		if (!(node->value.connection->right = parse_pipeline()))
			return (return_parse_error(&node));
		and_or = node;
	}
	return (and_or);
}
