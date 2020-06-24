/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   if_clause.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:35:38 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static t_command	*parse_else_statement()
{
	t_command	*else_clause;
	t_command	*then_part;
	
	if (!(then_part = parse_compound_list()))
	{
		if (!g_parser.status)
			g_parser.status = UNEXPECTED_TOKEN;
		return (NULL);
	}
	else_clause = command_new(IF_CLAUSE);
	else_clause->value.if_clause->then_part = then_part;
	return (else_clause);
}

static t_command	*parse_if_then_part()
{
	t_command	*if_then_clause;
	t_command	*if_part;
	t_command	*then_part;

	if (!(if_part = parse_compound_list()))
		return (NULL);
	if (!get_required_reserv_word(THEN))
	{
		if (!g_parser.status)
			g_parser.status = UNEXPECTED_TOKEN;
		complete_command_del(&if_part);
		return (NULL);
	}
	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_parser.token = get_next_token();
	if (!(then_part = parse_compound_list()))
	{
		if (!g_parser.status)
			g_parser.status = UNEXPECTED_TOKEN;
		complete_command_del(&if_part);
		return (NULL);
	}
	if_then_clause = command_new(IF_CLAUSE);
	if_then_clause->value.if_clause->if_part = if_part;
	if_then_clause->value.if_clause->then_part = then_part;
	return (if_then_clause);
}

/*
** else_part		: Elif compound_list Then compound_list
** 					| Elif compound_list Then compound_list else_part
** 					| Else compound_list
*/

t_command		*parse_else_part(void)
{
	t_command	*else_part;
	t_command	*elif_then_clause;
	t_command	*else_statement;
	t_command	*last;

	else_part = NULL;
	if (!g_parser.token)
		return (NULL);
	while (get_required_reserv_word(ELIF))
	{
		token_del(&g_parser.token);
		g_lexer.expect_reserv_word = true;
		g_parser.token = get_next_token();
		if (!(elif_then_clause = parse_if_then_part()))
		{
			if (!g_parser.status)
				g_parser.status = UNEXPECTED_TOKEN;
			complete_command_del(&else_part);
			return (NULL);
		}
		if (!else_part)
		{
			else_part = elif_then_clause;
			last = else_part;
		}
		else
		{
			last->value.if_clause->else_part = elif_then_clause;
			last = last->value.if_clause->else_part;
		}
	}
	if (get_required_reserv_word(ELSE))
	{
		token_del(&g_parser.token);
		g_lexer.expect_reserv_word = true;
		g_parser.token = get_next_token();
		if (!(else_statement = parse_else_statement()))
		{
			if (!g_parser.status)
				g_parser.status = UNEXPECTED_TOKEN;
			complete_command_del(&else_part);
			return (NULL);
		}
		if (else_part)
			last->value.if_clause->else_part = else_statement;
		else
			else_part = else_statement;
	}
	return (else_part);
}

/*
** typedef struct		s_if_clause
** {
** 	struct s_command	*if_part;
** 	struct s_command	*then_part;
** 	struct s_command	*else_part;
** }					t_if_clause;
**
** if x == 1; then		->		{ if_part	
** 		echo 1			->		then_part
** elif x == 2; then	->		else_part = { if_part
** 		echo 2			->					  then_part
** else					->					  else_part = { NULL
** 		echo 3			->									then_part
** 															NULL } } }			
*/

/*
** if_clause		: If compound_list Then compound_list else_part Fi
**            		| If compound_list Then compound_list           Fi
*/

t_command			*parse_if_clause(void)
{
	t_command	*if_clause;
	t_command	*else_part;
	int			old_linebreak_type;

	if_clause = NULL;
	else_part = NULL;
	if (!g_parser.token || g_parser.token->type != IF)
		return (NULL);
	old_linebreak_type = g_linebreak_type;
	g_linebreak_type = IF;
	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_parser.token = get_next_token();
	if (!(if_clause = parse_if_then_part())
	|| (!(else_part = parse_else_part()) && g_parser.status))
	{
		if (!g_parser.status)
			g_parser.status = UNEXPECTED_TOKEN;
		complete_command_del(&if_clause);
		return (NULL);
	}
	if_clause->value.if_clause->else_part = else_part;
	if (!get_required_reserv_word(FI))
	{
		if (!g_parser.status)
			g_parser.status = UNEXPECTED_TOKEN;
		complete_command_del(&if_clause);
		return (NULL);
	}
	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_parser.token = get_next_token();
	g_linebreak_type = old_linebreak_type;
	return (if_clause);
}
