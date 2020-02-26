/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 12:05:06 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/10 17:54:36 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		backslash_newline(void)
{
	if (!g_lexer.quote_st
	&& g_lexer.line[g_lexer.i] == '\\' && g_lexer.line[g_lexer.i + 1] == '\n')
	{
		ft_memmove(&g_lexer.line[g_lexer.i],
					&g_lexer.line[g_lexer.i + 2],
					ft_strlen(&g_lexer.line[g_lexer.i + 2]) + 1);
		g_lexer.line_cont = 1;
		return (1);
	}
	return (0);
}

int		backslash(void)
{
	if (g_lexer.line[g_lexer.i] == BSLASH && g_lexer.quote_st != SQUOTE)
	{
		if (!g_lexer.token)
			g_lexer.token = token_new(WORD);
		ft_dstr_add(g_lexer.token->value, g_lexer.line[g_lexer.i++]);
		ft_dstr_add(g_lexer.token->value, g_lexer.line[g_lexer.i++]);
		return (1);
	}
	return (0);
}

int		quote(void)
{
	if (g_lexer.line[g_lexer.i] == SQUOTE || g_lexer.line[g_lexer.i] == DQUOTE)
	{
		if (!g_lexer.token)
			g_lexer.token = token_new(WORD);
		if (!g_lexer.quote_st)
			g_lexer.quote_st = g_lexer.line[g_lexer.i];
		else if (g_lexer.quote_st == g_lexer.line[g_lexer.i])
			g_lexer.quote_st = 0;
		ft_dstr_add(g_lexer.token->value, g_lexer.line[g_lexer.i++]);
		return (1);
	}
	return (0);
}
