/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compound_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/10 00:08:46 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** term             : term separator and_or
**                  | term separator
**                  |                and_or
*/

t_command		*parse_term(void)
{
	t_command	*term;
	t_command	*command;
	int			sep_index;

	term = parse_and_or();
	command = term;
	while (command)
	{
		sep_index = g_lexer.i - 1;
		command->sep = parse_separator();
		if (g_parser.status != NOERR)
			break ;
		if (command->sep == NEWLINE)
			lx_line_insert_char(';', sep_index);
		if (command->sep)
			command->next = parse_and_or();
		command = command->next;
	}
	if (g_parser.status != NOERR)
		complete_command_del(&term);
	return (term);
}

/*
** compound_list    : linebreak term
**                  | linebreak term separator
*/

t_command		*parse_compound_list(void)
{
	t_command	*compound_list;
	t_command	*last;

	parse_linebreak();
	if (!(compound_list = parse_term()))
		return (NULL);
	last = compound_list;
	while (last->next)
		last = last->next;
	if (!last->sep)
		last->sep = parse_separator();
	return (compound_list);
}
