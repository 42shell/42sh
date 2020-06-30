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

static void	advance_and_parse_linebreak(void)
{
	int		old_linebreak_type;

	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_parser.token = get_next_token();
	old_linebreak_type = g_linebreak_type;
	g_linebreak_type = PIPE;
	parse_linebreak();
	g_linebreak_type = old_linebreak_type;
}

/*
** pipe_sequence    :                             command
**                  | pipe_sequence '|' linebreak command
*/

t_command	*parse_pipe_sequence(void)
{
	t_command	*pipe_sequence;
	t_command	*pipe_list;
	t_command	*command;

	if (!g_parser.token || !(command = parse_command()))
		return (NULL);
	pipe_list = command;
	while (g_parser.token && g_parser.token->type == PIPE)
	{
		advance_and_parse_linebreak();
		if (!(command->next = parse_command()))
			return (return_parse_error(&pipe_list));
		command = command->next;
		command->flags |= CMD_PIPE;
	}
	if (!pipe_list->next)
		return (pipe_list);
	pipe_sequence = command_new(PIPELINE);
	pipe_sequence->value.pipeline = pipe_list;
	return (pipe_sequence);
}

/*
** pipeline   	 	:      pipe_sequence
**                  | bang pipe_sequence
*/

t_command	*parse_pipeline(void)
{
	t_command	*pipeline;
	int			invert_return;

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
