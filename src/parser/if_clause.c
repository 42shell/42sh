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

/*
** At this point the if/elif token has already been parsed so we can use this
** fucntion for the first if/then statement in parse_if_clause and for elif/then
** statements in parse_else_part.
*/

t_command	*parse_if_then_statement(void)
{
	t_command	*if_then_statement;
	t_command	*if_part;
	t_command	*then_part;
	int			old_linebreak_type;

	if (!(if_part = parse_compound_list()))
		return (NULL);
	if (!get_required_reserv_word(THEN))
		return (return_parse_error(&if_part));
	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	old_linebreak_type = g_linebreak_type;
	g_linebreak_type = THEN;
	g_parser.token = get_next_token();
	if (!(then_part = parse_compound_list()))
		return (return_parse_error(&if_part));
	if_then_statement = command_new(IF_CLAUSE);
	if_then_statement->value.if_clause->if_part = if_part;
	if_then_statement->value.if_clause->then_part = then_part;
	g_linebreak_type = old_linebreak_type;
	return (if_then_statement);
}

/*
** if_clause		: If compound_list Then compound_list else_part Fi
**            		| If compound_list Then compound_list           Fi
*/

t_command	*parse_if_clause(void)
{
	t_command	*if_clause;
	t_command	*else_part;
	int			old_linebreak_type;

	if (!get_required_reserv_word(IF))
		return (NULL);
	old_linebreak_type = g_linebreak_type;
	g_linebreak_type = IF;
	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_parser.token = get_next_token();
	if (!(if_clause = parse_if_then_statement())
	|| (!(else_part = parse_else_part()) && g_parser.status))
		return (return_parse_error(&if_clause));
	if_clause->flags = CMD_IF;
	if_clause->value.if_clause->else_part = else_part;
	if (!get_required_reserv_word(FI))
		return (return_parse_error(&if_clause));
	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_parser.token = get_next_token();
	g_linebreak_type = old_linebreak_type;
	return (if_clause);
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
