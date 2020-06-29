/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   if_clause_else.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:35:38 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void			add_else_part(t_command **head, t_command **tail,
					t_command *else_part)
{
	if (!*head)
	{
		*head = else_part;
		*tail = *head;
	}
	else
	{
		(*tail)->value.if_clause->else_part = else_part;
		(*tail) = else_part;
	}
}

static t_command	*parse_else_statement(void)
{
	t_command	*else_statement;
	t_command	*then_part;

	g_linebreak_type = ELSE;
	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_parser.token = get_next_token();
	if (!(then_part = parse_compound_list()))
	{
		if (!g_parser.status)
			g_parser.status = UNEXPECTED_TOKEN;
		return (NULL);
	}
	else_statement = command_new(IF_CLAUSE);
	else_statement->value.if_clause->then_part = then_part;
	return (else_statement);
}

static t_command	*parse_elif_then_statement(void)
{
	t_command	*elif_then_statement;

	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_linebreak_type = ELIF;
	g_parser.token = get_next_token();
	if (!(elif_then_statement = parse_if_then_statement()))
		return (NULL);
	return (elif_then_statement);
}

/*
** else_part		: Elif compound_list Then compound_list
** 					| Elif compound_list Then compound_list else_part
** 					| Else compound_list
*/

t_command			*parse_else_part(void)
{
	t_command	*else_part;
	t_command	*else_part_tail;
	t_command	*elif_statement;
	t_command	*else_statement;
	int			old_linebreak_type;

	else_part = NULL;
	if (!g_parser.token)
		return (NULL);
	old_linebreak_type = g_linebreak_type;
	while (get_required_reserv_word(ELIF))
	{
		if (!(elif_statement = parse_elif_then_statement()))
			return (return_parse_error(&else_part));
		elif_statement->flags = CMD_ELIF;
		add_else_part(&else_part, &else_part_tail, elif_statement);
	}
	if (get_required_reserv_word(ELSE))
	{
		if (!(else_statement = parse_else_statement()))
			return (return_parse_error(&else_part));
		else_statement->flags = CMD_ELSE;
		add_else_part(&else_part, &else_part_tail, else_statement);
	}
	g_linebreak_type = old_linebreak_type;
	return (else_part);
}
