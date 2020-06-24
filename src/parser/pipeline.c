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

static t_command	*build_pipe_and_advance(t_command *left)
{
	t_command	*node;
	int			old_linebreak_type;

	node = command_new(CONNECTION);
	node->value.connection->connector = PIPE;
	node->value.connection->left = left;
	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_parser.token = get_next_token();
	old_linebreak_type = g_linebreak_type;
	g_linebreak_type = PIPE;
	parse_linebreak();
	g_linebreak_type = old_linebreak_type;
	return (node);
}

/*
** pipe_sequence    :                             command
**                  | pipe_sequence '|' linebreak command
*/

static t_command	*parse_pipe_sequence(void)
{
	t_command		*pipe_sequence;
	t_command		*node;

	if (!g_parser.token || !(pipe_sequence = parse_command()))
		return (NULL);
	while (g_parser.token
	&& g_parser.token->type == PIPE)
	{
		node = build_pipe_and_advance(pipe_sequence);
		if (!(node->value.connection->right = parse_command()))
		{
			if (!g_parser.status)
				g_parser.status = UNEXPECTED_TOKEN;
			command_del(&node);
			return (NULL);
		}
		pipe_sequence = node;
	}
	return (pipe_sequence);
}

/*
** pipeline   	 	:      pipe_sequence
**                  | bang pipe_sequence
*/

t_command			*parse_pipeline(void)
{
	t_command		*pipeline;
	int				invert_return;

	invert_return = 0;
	while (g_parser.token->type == BANG)
	{
		invert_return ^= CMD_INVERT_RETURN;
		token_del(&g_parser.token);
		g_lexer.expect_reserv_word = true;
		g_parser.token = get_next_token();
	}
	if (!(pipeline = parse_pipe_sequence()))
		return (NULL);
	pipeline->flags |= invert_return;
	return (pipeline);
}
