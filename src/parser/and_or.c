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

static void	advance_and_parse_linebreak(int type)
{
	int		old_linebreak_type;

	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_parser.token = get_next_token();
	old_linebreak_type = g_linebreak_type;
	g_linebreak_type = type;
	parse_linebreak();
	g_linebreak_type = old_linebreak_type;
}

/*
** and_or          :                         pipeline
**                 | and_or AND_IF linebreak pipeline
**                 | and_or OR_IF  linebreak pipeline
*/

t_command	*parse_and_or(void)
{
	t_command	*and_or;
	t_command	*and_or_list;
	t_command	*command;
	int			type;

	if (!g_parser.token || !(command = parse_pipeline()))
		return (NULL);
	and_or_list = command;
	while (g_parser.token
	&& (g_parser.token->type == AND_IF || g_parser.token->type == OR_IF))
	{
		type = g_parser.token->type;
		advance_and_parse_linebreak(type);
		if (!(command->next = parse_pipeline()))
			return (return_parse_error(&and_or_list));
		command = command->next;
		command->flags |= (type == AND_IF) ? CMD_AND_IF : CMD_OR_IF;
	}
	if (!and_or_list->next)
		return (and_or_list);
	and_or = command_new(AND_OR);
	and_or->value.and_or = and_or_list;
	return (and_or);
}
