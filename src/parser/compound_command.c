/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compound_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:35:38 by fratajcz         ###   ########.fr       */
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

/*
** subshell : '(' compound_list ')'
*/

t_command		*parse_subshell(void)
{
	t_command	*group;
	t_command	*compound_list;
	int			old_linebreak_type;

	if (!g_parser.token || g_parser.token->type != LBRACKET)
		return (NULL);
	old_linebreak_type = g_linebreak_type;
	g_linebreak_type = LBRACKET;
	g_parser.token = get_next_token();
	if (!(compound_list = parse_compound_list()))
		return (NULL);
	if (!g_parser.token || g_parser.token->type != RBRACKET)
	{
		if (!g_parser.status)
			g_parser.status = UNEXPECTED_TOKEN;
		complete_command_del(&compound_list);
		return (NULL);
	}
	group = command_new(GROUP);
	group->value.group->list = compound_list;
	token_del(&g_parser.token);
	g_parser.token = get_next_token();
	group->value.group->subshell = true;
	g_linebreak_type = old_linebreak_type;
	return (group);
}

/*
** brace_group : '{' compound_list '}'
*/

t_command		*parse_brace_group(void)
{
	t_command	*group;
	t_command	*compound_list;
	int			old_linebreak_type;

	if (!g_parser.token || g_parser.token->type != LBRACE)
		return (NULL);
	old_linebreak_type = g_linebreak_type;
	g_linebreak_type = LBRACE;
	g_parser.token = get_next_token();
	if (!(compound_list = parse_compound_list()))
		return (NULL);
	if (!g_parser.token || g_parser.token->type != RBRACE)
	{
		if (!g_parser.status)
			g_parser.status = UNEXPECTED_TOKEN;
		complete_command_del(&compound_list);
		return (NULL);
	}
	group = command_new(GROUP);
	group->value.group->list = compound_list;
	token_del(&g_parser.token);
	g_parser.token = get_next_token();
	g_linebreak_type = old_linebreak_type;
	return (group);
}


/*
** compound_command : brace_group
**                  | subshell
*/

t_command		*parse_compound_command(void)
{
	t_command	*compound_command;

	if (!(compound_command = parse_brace_group())
	&& !(compound_command = parse_subshell()))
		return (NULL);
	return (compound_command);
}
