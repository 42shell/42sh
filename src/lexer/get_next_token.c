/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/24 18:51:32 by fratajcz         ###   ########.fr       */
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
	return (PS2);
}

int				reset_lexer(void)
{
	if (g_lexer.token)
		token_del(&g_lexer.token);
	free(g_lexer.line);
	g_lexer.line = NULL;
	g_lexer.line_cont = 0;
	g_lexer.last_delim = 0;
	g_lexer.quote_st = 0;
	g_lexer.i = 0;
	return (0);
}

static t_token	*return_token(void)
{
	t_token	*ret;

	if (g_lexer.last_delim) //a token has been delimited
	{
		ret = g_lexer.token;
		g_lexer.token = NULL;
		g_lexer.last_delim = 0;
		return (ret);
	}
	else if (!g_lexer.nl_found) //escaped newlines or consecutives whitespaces in file in batch mode...
	{
		g_lexer.line_cont = 1;
		return (get_next_token());
	}
	g_lexer.nl_found = 0; //normal end of line
	g_lexer.quote_st = 0;
	return (NULL);
}

t_token			*get_next_token(void)
{
	if (g_parser.error)
		return (NULL);
	if (g_lexer.line_cont || g_lexer.quote_st)
	{
		if (g_shell.get_input(get_prompt()) != 0)
			return (NULL);
		g_lexer.line_cont = 0;
	}
	while (!g_lexer.last_delim && g_lexer.line[g_lexer.i])
	{
		lx_operator_next()
		|| lx_operator_end()
		|| lx_backslash()
		|| lx_quote()
		|| lx_operator_new()
		|| lx_newline()
		|| lx_blank()
		|| lx_word_next()
		|| lx_comment()
		|| lx_word_start();
	}
	return (return_token());
}
