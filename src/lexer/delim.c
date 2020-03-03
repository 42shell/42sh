/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delim.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/13 19:15:17 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int	delim_token(char delim)
{
	if (g_lexer.token)
	{
		if (is_operator_start(*g_lexer.token->value->str))
			g_lexer.token->type =
			get_operator_type(g_lexer.token->value->str);
		else if ((g_lexer.line[g_lexer.i] == '<' || g_lexer.line[g_lexer.i] == '>')
		&& ft_strisnbr(g_lexer.token->value->str))
			g_lexer.token->type = IO_NUMBER;
		g_lexer.last_delim = delim;
	}
	return (0);
}

int			lx_operator_end(void)
{
	if (g_lexer.token && !g_lexer.quote_st
	&& (is_operator_start(*g_lexer.token->value->str)
	&& (g_lexer.i > 0 ? is_operator_part(g_lexer.line[g_lexer.i - 1]) : 0))
	&& (!is_operator_part(g_lexer.line[g_lexer.i])
	|| !is_operator_next(g_lexer.token->value->str, g_lexer.line[g_lexer.i])))
	{
		delim_token(g_lexer.line[g_lexer.i]);
		return (1);
	}
	return (0);
}

int			lx_operator_new(void)
{
	if (g_lexer.token && !g_lexer.quote_st
	&& is_operator_start(g_lexer.line[g_lexer.i]))
	{
		if ((g_lexer.line[g_lexer.i] == '<' || g_lexer.line[g_lexer.i] == '>')
		&& ft_strisnbr(g_lexer.token->value->str))
			g_lexer.token->type = IO_NUMBER;
		delim_token(g_lexer.line[g_lexer.i]);
		return (1);
	}
	return (0);
}

int			lx_newline(void)
{
	if (!g_lexer.quote_st && g_lexer.line[g_lexer.i] == '\n')
	{
		g_lexer.nl_found = 1;
		if (!g_lexer.token)
		{
			g_lexer.token = token_new(NEWLINE);
			g_lexer.i++;
		}
		delim_token('\n');
		return (1);
	}
	return (0);
}

int			lx_blank(void)
{
	if (!g_lexer.quote_st
	&& (g_lexer.line[g_lexer.i] == '\t'
	|| g_lexer.line[g_lexer.i] == ' '))
	{
		if (g_lexer.token)
			delim_token(g_lexer.line[g_lexer.i]);
		while (g_lexer.line[g_lexer.i] == '\t'
		|| g_lexer.line[g_lexer.i] == ' ')
			g_lexer.i++;
		return (1);
	}
	return (0);
}
