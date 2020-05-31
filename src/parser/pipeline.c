/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 04:37:16 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:35:05 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** pipe_sequence    :                             command
**                  | pipe_sequence '|' linebreak command
*/

t_command	*parse_pipeline(void)
{
	t_command		*pipeline;
	t_command		*node;
	int				old_linebreak_type;

	if (!(pipeline = parse_command()))
		return (NULL);
	while (g_parser.token
	&& g_parser.token->type == PIPE)
	{
		node = command_new(CONNECTION);
		node->value.connection->connector = PIPE;
		node->value.connection->left = pipeline;
		token_del(&g_parser.token);
		g_parser.token = get_next_token();
		old_linebreak_type = g_linebreak_type;
		g_linebreak_type = PIPE;
		parse_linebreak();
		g_linebreak_type = old_linebreak_type;
		if (!(node->value.connection->right = parse_command()))
		{
			if (!g_parser.status)
				g_parser.status = UNEXPECTED_TOKEN;
			command_del(&node);
			return (NULL);
		}
		pipeline = node;
	}
	return (pipeline);	
}
