/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 04:37:16 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/10 00:13:35 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** linebreak		: newline_list
** 					| EMPTY
*/

int			parse_linebreak(void)
{
	parse_newline_list();
	return (0);
}

/*
** newline_list		: NEWLINE
** 					| NEWLINE newline_list
**
** returns true if there was a newline_list, false otherwise,
** replace the first newline with a space if we are in a line continuation,
** remove the following ones
** ex : ls |$  ->  ls | cat
**      $
**      cat
*/

static void	remove_newline(int nl_index, bool nl_replaced)
{
	if (!nl_replaced)
	{
		g_lexer.line[nl_index] = ' ';
		nl_replaced = 1;
	}
	else
	{
		ft_memmove(&g_lexer.line[nl_index],
					&g_lexer.line[nl_index + 1],
					ft_strlen(&g_lexer.line[nl_index]));
		g_lexer.i--;
	}
}

int			parse_newline_list(void)
{
	int		nl_index;
	bool	nl_replaced;

	nl_replaced = 0;
	if (!g_parser.token || g_parser.token->type != NEWLINE)
		return (0);
	while (g_parser.status == NOERR
	&& g_parser.token && g_parser.token->type == NEWLINE)
	{
		token_del(&g_parser.token);
		if (g_linebreak_type)
		{
			g_lexer.line_cont = g_linebreak_type;
			nl_index = g_lexer.i - 1;
		}
		g_lexer.expect_reserv_word = true;
		if ((g_parser.token = get_next_token()) && g_linebreak_type)
			remove_newline(nl_index, nl_replaced);
	}
	return (NEWLINE);
}

/*
** separator_op     : '&'
**                  | ';'
*/

int			parse_separator_op(void)
{
	enum e_token_type	type;

	if (!g_parser.token
	|| (g_parser.token->type != AMPERSAND && g_parser.token->type != SEMI))
		return (0);
	type = g_parser.token->type;
	token_del(&g_parser.token);
	g_lexer.expect_reserv_word = true;
	g_parser.token = get_next_token();
	return (type);
}

/*
** separator		: separator_op linebreak
** 					| newline_list
*/

int			parse_separator(void)
{
	enum e_token_type	type;

	if (g_parser.token == NULL)
		return (0);
	if ((type = parse_separator_op()))
	{
		parse_linebreak();
		return (type);
	}
	if (parse_newline_list())
		return (NEWLINE);
	return (0);
}
