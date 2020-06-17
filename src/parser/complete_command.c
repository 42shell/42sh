/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/09 23:03:00 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/10 00:03:52 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** -incorrect, complete_commands must be separated by a newline list,
**  commands separated by '\0' in batch mode will work.
** -as we read input line by line and cannot look ahead,
**  either we do like this, or any script must be terminated by \n.
**
** complete_command  | linebreak complete_command separator_op and_or linebreak
**                   | linebreak			      separator_op and_or linebreak
**                   | linebreak                  		       and_or linebreak
**					 | linebreak
*/

t_command	*parse_complete_command(void)
{
	t_command	*list;
	t_command	*command;

	parse_linebreak();
	g_lexer.expect_reserv_word = true;
	if (!(g_parser.token = get_next_token()))
		return (NULL);
	list = parse_and_or();
	command = list;
	while (command)
	{
		if (!command->sep && (command->sep = parse_separator_op()))
			command->next = parse_and_or();
		command = command->next;
	}
	if (g_parser.status == NOERR)
		parse_linebreak();
	if (g_parser.status != NOERR || g_parser.token)
	{
		if (!g_parser.status)
			g_parser.status = UNEXPECTED_TOKEN;
		complete_command_del(&list);
	}
	return (list);
}
