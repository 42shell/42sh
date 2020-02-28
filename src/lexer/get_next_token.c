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
	{
		token_del(&g_lexer.token);
		g_lexer.token = NULL;
	}
	free(g_lexer.line);
	g_lexer.line = NULL;
	g_lexer.token = NULL;
	g_lexer.line_cont = 0;
	g_lexer.token_is_delim = 0;
	g_lexer.quote_st = 0;
	g_lexer.i = 0;
	return (0);
}

static t_token	*return_token(void)
{
	t_token	*ret;

	if (g_lexer.token_is_delim)
	{
		ret = g_lexer.token;
		g_lexer.token = NULL;
		g_lexer.token_is_delim = 0;
		return (ret);
	}
	else if (g_lexer.line_cont || g_lexer.quote_st)
		return (get_next_token());
	g_lexer.quote_st = 0;
	return (NULL);
}

t_token			*get_next_token(void)
{
	if (g_lexer.line_cont || g_lexer.quote_st)
	{
		get_input(get_prompt());
		g_lexer.line_cont = 0;
	}
	if (g_parser.error || !g_lexer.line)
	{
		reset_lexer();
		return (NULL);
	}
	while (!g_lexer.token_is_delim && g_lexer.line[g_lexer.i])
	{
		backslash_newline()
		|| operator_next()
		|| operator_end()
		|| backslash()
		|| quote()
		|| operator_new()
		|| new_line()
		|| blank()
		|| word_next()
		|| comment()
		|| word_start();
	}
	return (return_token());
}
