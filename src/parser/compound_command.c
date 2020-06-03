/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compound_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:35:38 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int				lx_line_insert_char(char c, int index) //lexer folder
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

/*
** term             : term separator and_or
**                  | term separator
**                  |                and_or
*/

t_command		*parse_term(void)
{
	t_command	*term;
	t_command	*command;
	int			sep_index;

	term = parse_and_or();
	command = term;
	while (command)
	{
		sep_index = g_lexer.i - 1;
		command->sep = parse_separator();
		if (command->sep == NEWLINE)
			lx_line_insert_char(';', sep_index);
		if (command->sep)
			command->list = parse_and_or();
		command = command->list;
	}
	if (g_parser.status != NOERR)
		complete_command_del(&term);
	return (term);
}

/*
** compound_list    : linebreak term
**                  | linebreak term separator
*/

t_command		*parse_compound_list(void)
{
	t_command	*compound_list;
	t_command	*last;
	
	parse_linebreak();
	if (!(compound_list = parse_term()))
		return (NULL);
	last = compound_list;
	while (last->list)
		last = last->list;
	if (!last->sep)
		last->sep = parse_separator();
	return (compound_list);
}

/*
** subshell : '(' compound_list ')'
*/

t_command		*parse_subshell(void)
{
	t_command	*subshell;
	int			old_linebreak_type;

	if (!g_parser.token || g_parser.token->type != LBRACKET)
		return (NULL);
	old_linebreak_type = g_linebreak_type;
	g_linebreak_type = LBRACKET;
	g_parser.token = get_next_token();
	if (!(subshell = parse_compound_list()))
		return (NULL);
	if (!g_parser.token || g_parser.token->type != RBRACKET)
	{
		if (!g_parser.status)
			g_parser.status = UNEXPECTED_TOKEN;
		command_del(&subshell); //complete
		return (NULL);
	}
	token_del(&g_parser.token);
	g_parser.token = get_next_token();
	subshell->flags |= (CMD_GROUP | CMD_SUBSHELL);
	g_linebreak_type = old_linebreak_type;
	return (subshell);
}

/*
** brace_group : '{' compound_list '}'
*/

t_command		*parse_brace_group(void)
{
	t_command	*brace_group;
	int			old_linebreak_type;

	if (!g_parser.token || g_parser.token->type != LBRACE)
		return (NULL);
	old_linebreak_type = g_linebreak_type;
	g_linebreak_type = LBRACE;
	g_parser.token = get_next_token();
	if (!(brace_group = parse_compound_list()))
		return (NULL);
	if (!g_parser.token || g_parser.token->type != RBRACE)
	{
		if (!g_parser.status)
			g_parser.status = UNEXPECTED_TOKEN;
		command_del(&brace_group); //complete
		return (NULL);
	}
	token_del(&g_parser.token);
	g_parser.token = get_next_token();
	brace_group->flags |= CMD_GROUP;
	g_linebreak_type = old_linebreak_type;
	return (brace_group);
}


/*
** compound_command : brace_group
**                  | subshell
*/

t_command		*parse_compound_command(void)
{
	t_command	*compound_command;

	if (!(compound_command = parse_brace_group())
	&& !(compound_command = parse_subshell()))
		return (NULL);
	return (compound_command);
}
