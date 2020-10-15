/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/29 15:13:53 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*get_quote_string(enum e_quote_st quote)
{
	if (quote == BSLASH)
		return (ft_strdup("\\"));
	if (quote == SQUOTE)
		return (ft_strdup("'"));
	if (quote == DQUOTE)
		return (ft_strdup("\""));
	if (quote == PAREN)
		return (ft_strdup("("));
	if (quote == DPAREN)
		return (ft_strdup("$(("));
	return (ft_strdup("${"));
}

bool	is_redir(t_token *token)
{
	if (LESS <= token->type && token->type <= GREATAND)
		return (true);
	return (false);
}

int		lx_line_insert_char(char c, int index)
{
	char	*tmp;
	int		size;

	tmp = g_lexer.line;
	size = ft_strlen(g_lexer.line) + 1;
	g_lexer.line = ft_memrealloc(g_lexer.line, size, size + 1);
	ft_memmove(&g_lexer.line[index + 1],
				&g_lexer.line[index],
				ft_strlen(&g_lexer.line[index]));
	g_lexer.line[index] = c;
	g_lexer.i++;
	free(tmp);
	return (0);
}

int		lx_line_del_char(int index)
{
	ft_memmove(&g_lexer.line[index],
				&g_lexer.line[index + 1],
				ft_strlen(&g_lexer.line[index]));
	g_lexer.i--;
	return (0);
}

int		is_reserved_word(char *word)
{
	if (ft_strequ(word, "!"))
		return (BANG);
	if (ft_strequ(word, "{"))
		return (LBRACE);
	if (ft_strequ(word, "}"))
		return (RBRACE);
	if (ft_strequ(word, "if"))
		return (IF);
	if (ft_strequ(word, "then"))
		return (THEN);
	if (ft_strequ(word, "elif"))
		return (ELIF);
	if (ft_strequ(word, "else"))
		return (ELSE);
	if (ft_strequ(word, "fi"))
		return (FI);
	if (ft_strequ(word, "while"))
		return (WHILE);
	if (ft_strequ(word, "do"))
		return (DO);
	if (ft_strequ(word, "done"))
		return (DONE);
	return (0);
}
