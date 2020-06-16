/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 12:05:06 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/10 17:34:54 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		lx_backslash(void)
{
	if (g_lexer.line[g_lexer.i] == BSLASH && g_lexer.quote_st != SQUOTE
			&& get_bracket_status(g_lexer.brack_stack) != SQUOTE)
	{
		if (!g_lexer.token)
			g_lexer.token = token_new(WORD);
		ft_dstr_add(g_lexer.token->value, g_lexer.line[g_lexer.i++]);
		if (g_lexer.line[g_lexer.i])
			ft_dstr_add(g_lexer.token->value, g_lexer.line[g_lexer.i++]);
		return (1);
	}
	return (0);
}

int		lx_quote(void)
{
	if ((g_lexer.line[g_lexer.i] == SQUOTE || g_lexer.line[g_lexer.i] == DQUOTE)
			&& g_lexer.brack_stack->size == 0)
	{
		if (!g_lexer.token)
			g_lexer.token = token_new(WORD);
		if (!g_lexer.quote_st)
			g_lexer.quote_st = g_lexer.line[g_lexer.i];
		else if (g_lexer.quote_st == g_lexer.line[g_lexer.i])
			g_lexer.quote_st = NONE;
		ft_dstr_add(g_lexer.token->value, g_lexer.line[g_lexer.i++]);
		return (1);
	}
	return (0);
}
