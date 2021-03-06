/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:35:38 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int			check_bracket_lvl(void)
{
	if (++g_parser.bracket_lvl > BRACKET_NEST_LIMIT)
	{
		if (!g_parser.status)
			g_parser.status = BRACKET_NEST_LIMIT_REACHED;
		return (BRACKET_NEST_LIMIT_REACHED);
	}
	return (0);
}

static t_command	*build_subshell_and_advance(t_command *compound_list)
{
	t_command		*subshell;

	subshell = command_new(GROUP);
	subshell->value.compound_list = compound_list;
	subshell->flags |= CMD_SUBSHELL;
	token_del(&g_parser.token);
	g_parser.token = get_next_token();
	--g_parser.bracket_lvl;
	return (subshell);
}

/*
** subshell : '(' compound_list ')'
*/

t_command			*parse_subshell(void)
{
	t_command	*subshell;
	t_command	*compound_list;
	int			old_linebreak_type;

	if (!g_parser.token || g_parser.token->type != LPAREN
	|| check_bracket_lvl() != 0)
		return (NULL);
	old_linebreak_type = g_linebreak_type;
	g_linebreak_type = LPAREN;
	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_parser.token = get_next_token();
	if (!(compound_list = parse_compound_list()))
		return (NULL);
	if (!g_parser.token || g_parser.token->type != RPAREN)
		return (return_parse_error(&compound_list));
	subshell = build_subshell_and_advance(compound_list);
	g_linebreak_type = old_linebreak_type;
	return (subshell);
}

static t_command	*build_brace_group_and_advance(t_command *compound_list)
{
	t_command		*group;

	group = command_new(GROUP);
	group->value.compound_list = compound_list;
	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_parser.token = get_next_token();
	--g_parser.bracket_lvl;
	return (group);
}

/*
** brace_group : '{' compound_list '}'
*/

t_command			*parse_brace_group(void)
{
	t_command	*group;
	t_command	*compound_list;
	int			old_linebreak_type;

	if (!g_parser.token || g_parser.token->type != LBRACE
	|| check_bracket_lvl() != 0)
		return (NULL);
	old_linebreak_type = g_linebreak_type;
	g_linebreak_type = LBRACE;
	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_parser.token = get_next_token();
	if (!(compound_list = parse_compound_list()))
		return (NULL);
	if (!get_required_reserv_word(RBRACE))
		return (return_parse_error(&compound_list));
	group = build_brace_group_and_advance(compound_list);
	g_linebreak_type = old_linebreak_type;
	return (group);
}
