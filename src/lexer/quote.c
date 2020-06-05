/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 12:05:06 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/05 23:46:45 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		lx_backslash(void)
{
	char	*quote_st;

	quote_st = get_bracket_status(g_lexer.brack_stack) != '\0'
		? &g_lexer.inner_quote_st : &g_lexer.quote_st;
	if (g_lexer.line[g_lexer.i] == BSLASH && *quote_st != SQUOTE)
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
	char	*quote_st;

	if (g_lexer.line[g_lexer.i] == SQUOTE || g_lexer.line[g_lexer.i] == DQUOTE)
	{
		quote_st = get_bracket_status(g_lexer.brack_stack) != '\0'
			? &g_lexer.inner_quote_st : &g_lexer.quote_st;
		if (!g_lexer.token)
			g_lexer.token = token_new(WORD);
		if (!*quote_st)
			*quote_st = g_lexer.line[g_lexer.i];
		else if (*quote_st == g_lexer.line[g_lexer.i])
			*quote_st = 0;
		ft_dstr_add(g_lexer.token->value, g_lexer.line[g_lexer.i++]);
		return (1);
	}
	return (0);
}
