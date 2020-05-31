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

/*
** and_or          :                         pipeline
**                 | and_or AND_IF linebreak pipeline
**                 | and_or OR_IF  linebreak pipeline
*/

/*
** unexpected token 'EOF' linebreak (null) ?
*/

t_command	*parse_and_or(void)
{
	t_command		*and_or;
	t_command		*node;
	int				old_linebreak_type;

	if (!(and_or = parse_pipeline()))
		return (NULL);
	while (g_parser.token &&
	(g_parser.token->type == AND_IF || g_parser.token->type == OR_IF))
	{
		node = command_new(CONNECTION);
		node->value.connection->connector = g_parser.token->type;
		node->value.connection->left = and_or;
		token_del(&g_parser.token);
		g_parser.token = get_next_token();
		old_linebreak_type = g_linebreak_type;
		g_linebreak_type = node->value.connection->connector;
		parse_linebreak();
		g_linebreak_type = old_linebreak_type;
		if (!(node->value.connection->right = parse_pipeline()))
		{
			if (!g_parser.status)
				g_parser.status = UNEXPECTED_TOKEN;//set_parser_status(int status)
			command_del(&node);
			return (NULL);
		}
		and_or = node;
	}
	return (and_or);	
}
