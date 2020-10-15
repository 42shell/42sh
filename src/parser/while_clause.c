/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   while_clause.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:35:38 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_command	*parse_do_group(void)
{
	t_command	*do_group;
	int			old_linebreak_type;

	if (!get_required_reserv_word(DO))
		return (NULL);
	old_linebreak_type = g_linebreak_type;
	g_linebreak_type = DO;
	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_parser.token = get_next_token();
	if (!(do_group = parse_compound_list()))
		return (NULL);
	if (!get_required_reserv_word(DONE))
		return (return_parse_error(&do_group));
	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_parser.token = get_next_token();
	g_linebreak_type = old_linebreak_type;
	return (do_group);
}

t_command	*parse_while_clause(void)
{
	t_command	*while_clause;
	t_command	*if_part;
	t_command	*then_part;
	int			old_linebreak_type;

	if (!get_required_reserv_word(WHILE))
		return (NULL);
	old_linebreak_type = g_linebreak_type;
	g_linebreak_type = WHILE;
	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_parser.token = get_next_token();
	if (!(if_part = parse_compound_list()))
		return (NULL);
	if (!(then_part = parse_do_group()))
		return (return_parse_error(&if_part));
	while_clause = command_new(WHILE_CLAUSE);
	while_clause->value.if_clause->if_part = if_part;
	while_clause->value.if_clause->then_part = then_part;
	g_linebreak_type = old_linebreak_type;
	return (while_clause);
}
