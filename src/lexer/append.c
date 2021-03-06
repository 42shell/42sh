/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 21:27:56 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 04:57:38 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		lx_operator_next(void)
{
	if (g_lexer.token && !g_lexer.quote_st
	&& get_bracket_status(g_lexer.brack_stack) == '\0'
	&& (g_lexer.i != 0 ? is_operator_part(g_lexer.line[g_lexer.i - 1]) : 0)
	&& is_operator_part(g_lexer.line[g_lexer.i])
	&& is_operator_next(g_lexer.token->value->str, g_lexer.line[g_lexer.i]))
	{
		ft_dstr_add(g_lexer.token->value, g_lexer.line[g_lexer.i]);
		g_lexer.i++;
		return (1);
	}
	return (0);
}

int		lx_word_next(void)
{
	bool can_open;

	if (g_lexer.token)
	{
		can_open = (g_lexer.quote_st == NONE || g_lexer.quote_st == DQUOTE);
		set_bracket_status(g_lexer.line, g_lexer.i, g_lexer.brack_stack,
							can_open);
		ft_dstr_add(g_lexer.token->value, g_lexer.line[g_lexer.i]);
		if (g_lexer.quote_st == BSLASH)
			g_lexer.quote_st &= ~BSLASH;
		g_lexer.i++;
		return (1);
	}
	return (0);
}

int		lx_comment(void)
{
	if (g_lexer.line[g_lexer.i] == '#')
	{
		while (g_lexer.line[g_lexer.i] && g_lexer.line[g_lexer.i] != '\n')
			g_lexer.i++;
		return (1);
	}
	return (0);
}

/*
** This is the only case where we create a new token, most of the others
** functions will not enter their if statement if curr_tok is set to NULL.
*/

int		lx_word_start(void)
{
	bool can_open;

	can_open = (g_lexer.quote_st == NONE || g_lexer.quote_st == DQUOTE);
	set_bracket_status(g_lexer.line, g_lexer.i, g_lexer.brack_stack, can_open);
	g_lexer.token = token_new(WORD);
	ft_dstr_add(g_lexer.token->value, g_lexer.line[g_lexer.i]);
	g_lexer.i++;
	return (1);
}
