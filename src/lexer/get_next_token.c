/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:29:08 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char		*get_prompt(void)
{
	if (g_lexer.quote_st == SQUOTE)
		return (PSQ);
	else if (g_lexer.quote_st == DQUOTE)
		return (PSD);
	else if (g_lexer.line_cont == PIPE)
		return (PSP);
	else if (g_lexer.line_cont == AND_IF)
		return (PSA);
	else if (g_lexer.line_cont == OR_IF)
		return (PSO);
	else if (g_lexer.line_cont == LBRACKET)
		return (PSB);
	else if (g_lexer.line_cont == LBRACE)
		return (PSC);
	return (PS2);
}

int				reset_lexer(void)
{
	token_del(&g_lexer.token);
	free(g_lexer.line);
	g_lexer.line = NULL;
	g_lexer.line_cont = 0;
	g_lexer.token_delimited = 0;
	g_lexer.nl_found = 0;
	g_lexer.end_of_input = 0;
	g_lexer.quote_st = 0;
	g_lexer.expect_reserv_word = false;
	g_lexer.i = 0;
	return (0);
}

static t_token	*return_token(void)
{
	t_token	*ret;

	if (g_lexer.token_delimited)
	{
		ret = g_lexer.token;
		g_lexer.token = NULL;
		g_lexer.token_delimited = 0;
		g_lexer.quote_st = 0;
		return (ret);
	}
	if (!g_lexer.nl_found)
	{
		g_lexer.line_cont = 1;
		return (get_next_token());
	}
	return (NULL);
}

t_token			*get_next_token(void)
{
	int		ret;

	if (g_parser.status != NOERR || !g_lexer.line)
		return (NULL);
	if (g_lexer.line_cont || g_lexer.quote_st)
	{
		g_lexer.nl_found = 0;
		g_lexer.end_of_input = 0;
		g_lexer.line_cont = 0;
		if ((ret = g_shell.get_input(get_prompt(), false)) != 0)
		{
			if (ret == INPUT_EOF)
			{
				g_lexer.token_delimited = true;
				return (return_token());
			}
			return (NULL);
		}
	}
	while (!g_lexer.end_of_input && !g_lexer.token_delimited)
	{
		lx_end() || lx_operator_next() || lx_operator_end() || lx_backslash()
		|| lx_quote() || lx_operator_new() || lx_newline() || lx_blank()
		|| lx_word_next() || lx_comment() || lx_word_start();
	}
	return (return_token());
}
