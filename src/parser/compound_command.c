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

/*
** term             : term separator and_or
**                  | term separator
**                  |                and_or
*/

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

t_command		*parse_term(void)
{
	t_command	*term;
	t_command	*command;
	int			nl_index;

	term = parse_and_or();
	command = term;
	while (command)
	{
		nl_index = g_lexer.i - 1;
		command->sep = parse_separator();
		if (command->sep == NEWLINE)
			lx_line_insert_char(';', nl_index);
		if (command->sep)
			command->next = parse_and_or();
		command = command->next;
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
	while (last->next)
		last = last->next;
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

	if (!g_parser.token || g_parser.token->type != OBRACKET)
		return (NULL);
	old_linebreak_type = g_linebreak_type;
	g_linebreak_type = OBRACKET;
	g_parser.token = get_next_token();
	if (!(subshell = parse_compound_list()))
		return (NULL);
	if (!g_parser.token || g_parser.token->type != CBRACKET)
	{
		if (!g_parser.status)
			g_parser.status = UNEXPECTED_TOKEN;
		command_del(&subshell); //complete
		return (NULL);
	}
	token_del(&g_parser.token);
	g_parser.token = get_next_token();
	subshell->flags |= CMD_SUBSHELL;
	g_linebreak_type = old_linebreak_type;
	return (subshell);
}


/*
** compound_command : simple_command
**                  | subshell
**                  | brace_group
*/

t_command		*parse_compound_command(void)
{
	t_command	*compound_command;

	//brace
	if (!(compound_command = parse_subshell()))
		return (NULL);
	return (compound_command);
}
