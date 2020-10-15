/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 04:58:14 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int				reset_lexer(void)
{
	token_del(&g_lexer.token);
	free(g_lexer.line);
	array_destroy(g_lexer.brack_stack);
	g_lexer.brack_stack = array_new(4);
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

static t_token	*end_of_input(int ret)
{
	char		*err;

	if (ret == INPUT_EOF)
	{
		if (g_lexer.quote_st != 0 || g_lexer.brack_stack->size != 0)
		{
			err = get_quote_string(g_lexer.brack_stack->size ?
						*(enum e_quote_st *)g_lexer.brack_stack->array[0] :
						(enum e_quote_st)g_lexer.quote_st);
			ft_dprintf(2, "42sh: unexpected EOF while looking for "
						"matching '%s'\n", err);
			g_parser.status = UNEXPECTED_TOKEN;
			free(err);
			return (NULL);
		}
		if (!delim_token())
			return (NULL);
		return (return_token());
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
		if ((ret = g_shell.get_input(get_prompt(), false)) != 0)
			return (end_of_input(ret));
		g_lexer.line_cont = 0;
	}
	while (!g_lexer.end_of_input && !g_lexer.token_delimited)
	{
		if (!(lx_end() || lx_operator_next() || lx_operator_end()
		|| lx_backslash() || lx_quote() || lx_operator_new() || lx_newline()
		|| lx_blank() || lx_word_next() || lx_comment() || lx_word_start()))
			return (NULL);
	}
	return (return_token());
}
