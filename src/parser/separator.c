/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 04:37:16 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/09 21:33:36 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** linebreak		: newline_list
** 					| EMPTY
**
** get input if empty, last_token_type determine the prompt that
** readline will display
*/

void		parse_linebreak(int last_token_type)
{
	parse_newline_list();
	while (g_parser.status == NOERR && g_parser.token == NULL)
	{
		g_lexer.line_cont = last_token_type;
		g_parser.token = get_next_token();
		if (g_parser.token != NULL && g_parser.token->type == NEWLINE)
			token_del(&g_parser.token);
	}
}

/*
** newline_list		: NEWLINE
** 					| NEWLINE newline_list
**
** returns true if there was a newline_list, false otherwise
*/

bool		parse_newline_list(void)
{
	if (g_parser.status != NOERR || g_parser.token == NULL)
		return (false);
	while (g_parser.token && g_parser.token->type == NEWLINE)
	{
		token_del(&g_parser.token);
		g_parser.token = get_next_token();
	}
	return (true);
}

/*
** separator		: separator_op newline_list
** 					| newline_list
**
** returns the separator token type or -1 if error or no separator
*/

int			parse_separator(void)
{
	enum e_token_type type;

	if (g_parser.status != NOERR || g_parser.token == NULL)
		return (-1);
	type = g_parser.token->type;
	if (type != AMPERSAND && type != SEMI)
		return (-1);
	token_del(&g_parser.token);
	g_parser.token = get_next_token();
	if (parse_newline_list() == false)
	{
		g_parser.status = UNEXPECTED_TOKEN;
		return (-1);
	}
	return (type);
}
