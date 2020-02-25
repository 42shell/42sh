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

int				reset_lexer(void)
{
	if (g_lexer.token)
	{
		token_del(&g_lexer.token);
		g_lexer.token = NULL;
	}
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
	else if (g_lexer.line_cont
	|| (g_lexer.quote_st && (g_lexer.line_cont = 1)))
		return (get_next_token());
	g_lexer.quote_st = 0;
	return (NULL);
}

t_token			*get_next_token(void)
{
	if (g_lexer.line_cont)
	{
		g_lexer.line_cont = 0;
		get_input(PS2);
	}
	while (!g_lexer.token_is_delim && g_line[g_lexer.i])
	{
		backslash_newline()
		|| operator_next()
		|| operator_end()
		|| backslash()
		|| quote()
		|| operator_new()
		|| blank()
		|| word_next()
		|| comment()
		|| word_start();
	}
	return (return_token());
}
