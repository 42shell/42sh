/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/01 16:06:10 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int		is_valid_argv(t_token *token)
{
	return (token
	&& (token->type == WORD 
	|| token->type == IO_NUMBER
	|| is_redir(token)));
}

static void		add_command_word(t_command *command, t_token *word)
{
	t_token		*ptr;

	if (!command->words)
		command->words = word;
	else
	{
		ptr = command->words;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = word;
	}
}

static void		add_command_redir(t_command *command, t_redir *redir)
{
	t_redir		*ptr;

	if (!command->redirs)
		command->redirs = redir;
	else
	{
		ptr = command->redirs;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = redir;
	}
}

static int		set_tokens(t_command *command)
{
	t_redir		*redir;

	while (is_valid_argv(g_parser.token))
	{
		if (g_parser.token->type == WORD)
		{
			add_command_word(command, g_parser.token);
			g_parser.token = get_next_token();
		}
		else if ((redir = parse_io_redirect()))
			add_command_redir(command, redir);
		else
			return (-1);
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

t_node		*parse_simple_command(void)
{
	t_node		*simple_command;
	t_command	*tokens;

	if (!is_valid_argv(g_parser.token))
	{
		g_parser.error = g_parser.error ? g_parser.error : NO_CMD_BEFORE_OP;
		return (NULL);
	}
	else if ((tokens = (t_command *)ft_xmalloc(sizeof(t_command)))
	&& set_tokens(tokens) == -1)
	{
		command_del(&tokens);
		return (NULL);
	}
	simple_command = (t_node *)ft_xmalloc(sizeof(t_node));
	simple_command->data = tokens;
	simple_command->type = NODE_SMPL_CMD;
	return (simple_command);
}

t_node		*parse_command(void)
{
	t_node	*command;
	t_node	*simple_command;

	if (!(simple_command = parse_simple_command()))
		return (NULL);
	command = (t_node *)ft_xmalloc(sizeof(t_node));
	command->left = simple_command;
	command->type = NODE_CMD;
	return (command);
}
