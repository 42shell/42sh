/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 04:37:16 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/01 16:08:26 by fratajcz         ###   ########.fr       */
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
	while (!g_parser.error && !g_parser.token)
	{
		g_lexer.line_cont = last_token_type;
		g_parser.token = get_next_token();
	}
}

/*
** newline_list		: NEWLINE
** 					| NEWLINE newline_list
*/

void		parse_newline_list(void)
{
	if (g_parser.error)
		return ;
	while (g_parser.token && g_parser.token->type == NEWLINE)
	{
		token_del(&g_parser.token);
		g_parser.token = get_next_token();
	}
}

/*
** separator_op		: '&'
**					| ';'
*/

t_node		*parse_separator_op(void)
{
	t_node	*sep_op;

	if (g_parser.error)
		return (NULL);
	if (g_parser.token
	&& (g_parser.token->type == SEMI || g_parser.token->type == AMPERSAND))
	{
		sep_op = (t_node *)ft_xmalloc(sizeof(t_node));
		sep_op->type = g_parser.token->type == SEMI ? NODE_SEMI : NODE_AMPER;
		token_del(&g_parser.token);
		g_parser.token = get_next_token();
		return (sep_op);
	}
	return (NULL);
}

/*
** separator		: separator_op newline_list
** 					| newline_list
*/

t_node		*parse_separator(void)
{
	t_node	*sep_op;

	sep_op = NULL;
	if (g_parser.error)
		return (NULL);
	if (g_parser.token)
	{
		sep_op = parse_separator_op();
		parse_newline_list();
	}
	return (sep_op);
}
