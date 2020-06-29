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

void	format_simple_command(t_dstr *buf, t_command *command)
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

void	format_pipeline(t_dstr *buf, t_command *command)
{
	t_command	*pipeline;

	pipeline = command->value.pipeline;
	while (pipeline)
	{
		if (pipeline->flags & CMD_PIPE)
			ft_dstr_append(buf, " | ");
		format_command(buf, pipeline);
		pipeline = pipeline->next;
	}
}

void	format_and_or(t_dstr *buf, t_command *command)
{
	t_command	*and_or;

	and_or = command->value.and_or;
	while (and_or)
	{
		if (and_or->flags & CMD_AND_IF)
			ft_dstr_append(buf, " && ");
		else if (and_or->flags & CMD_OR_IF)
			ft_dstr_append(buf, " || ");
		format_command(buf, and_or);
		and_or = and_or->next;
	}
}

void	format_command(t_dstr *buf, t_command *command)
{
	if (!command || !buf)
		return ;
	if (command->type == AND_OR)
		format_and_or(buf, command);
	else if (command->type == PIPELINE)
		format_pipeline(buf, command);
	else if (command->type == GROUP)
		format_group(buf, command);
	else if (command->type == SIMPLE)
		format_simple_command(buf, command);
	else if (command->type == IF_CLAUSE)
		format_if_clause(buf, command);
	else if (command->type == WHILE_CLAUSE)
		format_while_clause(buf, command);
}
