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

bool	is_operator_start(char c)
{
	return (c == '<' || c == '>' || c == '&' || c == ';' || c == '|'
	|| c == '(' || c == ')');
}

bool	is_operator_part(char c)
{
	return ((is_operator_start(c)) || c == '-');
}

bool	is_redir(t_token *token)
{
	if (LESS <= token->type && token->type <= GREATAND)
		return (true);
	return (false);
}

bool	is_operator_next(char *ope, char c)
{
	if (is_operator_part(c))
	{
		if (c == '<')
			return (ft_strequ(ope, "<"));
		else if (c == '>')
			return (ft_strequ(ope, ">"));
		else if (c == '&')
			return (ft_strequ(ope, "<")
			|| ft_strequ(ope, ">")
			|| ft_strequ(ope, "&"));
		else if (c == '|')
			return (ft_strequ(ope, "|"));
	}
	return (0);
}

int		get_operator_type(char *ope)
{
	if (ft_strequ(ope, "<"))
		return (LESS);
	if (ft_strequ(ope, ">"))
		return (GREAT);
	if (ft_strequ(ope, "<<"))
		return (DLESS);
	if (ft_strequ(ope, ">>"))
		return (DGREAT);
	if (ft_strequ(ope, "<&"))
		return (LESSAND);
	if (ft_strequ(ope, ">&"))
		return (GREATAND);
	if (ft_strequ(ope, "&"))
		return (AMPERSAND);
	if (ft_strequ(ope, "&&"))
		return (AND_IF);
	if (ft_strequ(ope, "|"))
		return (PIPE);
	if (ft_strequ(ope, "||"))
		return (OR_IF);
	if (ft_strequ(ope, ";"))
		return (SEMI);
	if (ft_strequ(ope, "("))
		return (LBRACKET);
	if (ft_strequ(ope, ")"))
		return (RBRACKET);
	return (0);
}

int		lx_line_insert_char(char c, int index)
{
	char		*tmp;

	tmp = g_lexer.line;
	g_lexer.line = ft_memrealloc(g_lexer.line,
								ft_strlen(g_lexer.line) + 1,
								ft_strlen(g_lexer.line) + 2);
	free(tmp);
	ft_memmove(&g_lexer.line[index + 1],
				&g_lexer.line[index],
				ft_strlen(&g_lexer.line[index]));
	g_lexer.line[index] = c;
	g_lexer.i++;
	return (0);
}

int		is_reserved_word(char *word)
{
	if (ft_strequ(word, "{"))
		return (LBRACE);
	if (ft_strequ(word, "}"))
		return (RBRACE);
	return (0);
}
