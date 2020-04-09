/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/09 18:17:08 by fratajcz         ###   ########.fr       */
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

static void		add_word(t_simple_cmd *simple, t_token *word, bool assignment)
{
	t_token		*ptr;

	if (assignment)
	{
		if (simple->assign_list == NULL)
			simple->assign_list = word;
		else
		{
			ptr = simple->assign_list;
			while (ptr->next)
				ptr = ptr->next;
			ptr->next = word;
		}
	}
	else
		array_append(simple->argv, word->value->str);
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
	bool		assignment;

	prefix = true;
	while (g_parser.token)
	{
		if (g_parser.token->type == WORD)
		{
			if (prefix)
			{
				assignment = is_assignment(g_parser.token->value->str);
				if (!assignment)
					prefix = false;
			}
			add_word(simple, g_parser.token, assignment);
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
** for the moment,
** command ≃ simple_command
**
** command			: WORD
** 					| WORD command
**					| io_redir
**					| io_redir command
**
** returns a t_command containing argv array and a list of redirections.
*/

t_command		*parse_simple_command(void)
{
	t_command		*cmd;

	if (g_parser.token->type != WORD && g_parser.token->type != IO_NUMBER
			&& !is_redir(g_parser.token))
		return (NULL);
	cmd = command_new(SIMPLE);
	if (set_tokens(cmd->value.simple) == -1)
	{
		command_del(&cmd);
		return (NULL);
	}
	return (cmd);
}

t_command		*parse_command(void)
{
	return (parse_simple_command());
}
