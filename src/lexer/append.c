/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 21:27:56 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/05 16:21:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		lx_operator_next(void)
{
	if (g_lexer.token && !g_lexer.quote_st && !g_lexer.brace_open
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

void	set_bracket_status(void)
{
	if (g_lexer.inner_quote_st == NONE)
	{
		if (g_lexer.brace_open && g_lexer.line[g_lexer.i] == '}')
			g_lexer.brace_open = false;
		else if (g_lexer.dparen_open && g_lexer.line[g_lexer.i] == ')'
				&& g_lexer.line[g_lexer.i] == ')')
			g_lexer.dparen_open = false;
	}
	if (g_lexer.quote_st == NONE || g_lexer.quote_st == DQUOTE)
	{
		if (g_lexer.line[g_lexer.i] == '$')
		{
			if (g_lexer.line[g_lexer.i + 1] == '{')
				g_lexer.brace_open = true;
			if (g_lexer.line[g_lexer.i + 1] == '('
					&& g_lexer.line[g_lexer.i + 2] == '(') 
				g_lexer.dparen_open = true;
		}
	}
}


int		lx_word_next(void)
{
	set_bracket_status();
	if (g_lexer.token)
	{
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
	set_bracket_status();
	g_lexer.token = token_new(WORD);
	ft_dstr_add(g_lexer.token->value, g_lexer.line[g_lexer.i]);
	g_lexer.i++;
	return (1);
}
