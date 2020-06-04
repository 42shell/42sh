/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/08 18:35:44 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:35:57 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static bool		is_assignment(char *str)
{
	if (ft_isdigit(*str))
		return (false);
	while (*str && *str != '=')
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (false);
		str++;
	}
	return (*str == '=');
}

static void		add_word(t_token **token_list, t_token *word)
{
	t_token		*ptr;

	if (*token_list == NULL)
		*token_list = word;
	else
	{
		ptr = (*token_list);
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = word;
	}
}

static void		add_redir(t_simple_cmd *simple, t_redir *redir)
{
	t_redir		*ptr;

	if (simple->redirs == NULL)
		simple->redirs = redir;
	else
	{
		ptr = simple->redirs;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = redir;
	}
}

static int		set_tokens(t_simple_cmd *simple)
{
	t_redir		*redir;
	bool		prefix;
	bool		assign;

	prefix = true;
	while (g_parser.token)
	{
		if (g_parser.token->type == WORD)
		{
			if (prefix)
			{
				assign = is_assignment(g_parser.token->value->str);
				if (!assign)
					prefix = false;
			}
			add_word(assign ? &simple->assigns : &simple->args, g_parser.token);
			g_parser.token = get_next_token();
		}
		else if ((redir = parse_io_redirect()))
			add_redir(simple, redir);
		else
			break ;
	}
	return (0);
}

/*
** simple_command	: WORD
** 					| WORD command
**					| io_redir
**					| io_redir command
*/

t_command		*parse_simple_command(void)
{
	t_command	*cmd;

	if (g_parser.token == NULL
	|| g_parser.token->type == NEWLINE)
		return (NULL);
	else if (g_parser.token->type != WORD && g_parser.token->type != IO_NUMBER
	&& !is_redir(g_parser.token))
		return (NULL);
	cmd = command_new(SIMPLE);
	set_tokens(cmd->value.simple);
	return (cmd);
}
