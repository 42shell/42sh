/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 04:58:14 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char	*get_prompt_quote(enum e_quote_st bracket_status)
{
	if (g_lexer.quote_st == SQUOTE || bracket_status == SQUOTE)
		return (PSQ);
	if (g_lexer.quote_st == DQUOTE || bracket_status == DQUOTE)
		return (PSDQ);
	if (bracket_status == BRACE)
		return (PSB);
	if (bracket_status == DPAREN)
		return (PSDP);
	if (bracket_status == PAREN)
		return (PSP);
	return (PS2);
}

char		*get_prompt(void)
{
	enum e_quote_st	bracket_status;

	bracket_status = get_bracket_status(g_lexer.brack_stack);
	if (g_lexer.quote_st || bracket_status)
		return (get_prompt_quote(bracket_status));
	if (g_lexer.line_cont == PIPE)
		return (PSP);
	if (g_lexer.line_cont == AND_IF)
		return (PSA);
	if (g_lexer.line_cont == OR_IF)
		return (PSO);
	if (g_lexer.line_cont == LBRACKET)
		return (PSB);
	if (g_lexer.line_cont == LBRACE)
		return (PSC);
	if (g_lexer.line_cont == IF)
		return (PSI);
	if (g_lexer.line_cont == THEN)
		return (PST);
	if (g_lexer.line_cont == ELIF)
		return (PSEI);
	if (g_lexer.line_cont == ELSE)
		return (PSE);
	if (g_lexer.line_cont == WHILE)
		return (PSW);
	if (g_lexer.line_cont == DO)
		return (PSD);
	return (PS2);
}
