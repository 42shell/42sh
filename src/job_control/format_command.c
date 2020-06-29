/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 17:55:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** deprecated for the moment
*/

static void	format_simple_command(t_dstr *buf, t_command *command)
{
	t_redir		*redir;
	t_token		*arg;

	redir = command->value.simple->redirs;
	arg = command->value.simple->args;
	while (arg != NULL)
	{
		ft_dstr_append(buf, arg->value->str);
		if ((arg = arg->next) != NULL || redir)
			ft_dstr_append(buf, " ");
	}
	while (redir)
	{
		if (redir->left_op)
			ft_dstr_append(buf, redir->left_op->value->str);
		ft_dstr_append(buf, redir->operator->value->str);
		ft_dstr_append(buf, redir->right_op->value->str);
		if ((redir = redir->next))
			ft_dstr_append(buf, " ");
	}
}

static void	format_group(t_dstr *buf, t_command *command)
{
	t_command	*cmd;

	cmd = command->value.compound_list;
	ft_dstr_append(buf, (command->flags & CMD_SUBSHELL) ? "( " : "{ ");
	while (cmd)
	{
		format_command(buf, cmd);
		if (cmd->sep == AMPERSAND)
		{
			ft_dstr_append(buf, " &");
			if (cmd->next)
				ft_dstr_append(buf, " ");
		}
		else if (cmd->next)
			ft_dstr_append(buf, "; ");
		else if (!(command->flags & CMD_SUBSHELL))
			ft_dstr_append(buf, ";");
		cmd = cmd->next;
	}
	ft_dstr_append(buf, (command->flags & CMD_SUBSHELL) ? " )" : " }");
}

void		format_command(t_dstr *buf, t_command *command)
{
	if (!command || !buf)
		return ;
	if (command->type == GROUP)
		return (format_group(buf, command));
	if (command->type == AND_OR)
		ft_dstr_append(buf, (command->flags & CMD_AND_IF) ? " && " : " || ");//format_and_or
	if (command->type == PIPE)
		ft_dstr_append(buf, " | ");//format_pipeline
	if (command->type == SIMPLE)
		return (format_simple_command(buf, command));
	return ;
}
